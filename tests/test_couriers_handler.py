import pytest


def format_hours(hours: list[str]) -> str:
    return "'{\"" + "\",\"".join(hours) + "\"}'"


@pytest.mark.pgsql("lavka", files=["init.sql"])
async def test_get_couriers(service_client, pgsql):
    def check_couriers(couriers_json, cursor):
        for courier_json in couriers_json:
            cursor.execute("""
                    SELECT id FROM lavka.couriers
                    WHERE id = {} AND type = '{}' AND regions = '{}' AND working_hours = {};
                    """.format(str(courier_json['courier_id']), courier_json['courier_type'],
                               "{" + ", ".join(str(x)
                                               for x in courier_json['regions']) + "}",
                               format_hours(courier_json['working_hours'])))
            res = cursor.fetchall()
            assert len(res) == 1

    # all presented
    response = await service_client.get('/couriers?limit=5')
    assert response.status == 200

    data = response.json()
    assert data['limit'] == 5
    assert data['offset'] == 0
    assert len(data['couriers']) == 5

    cursor = pgsql['lavka'].cursor()
    check_couriers(data['couriers'], cursor)

    # with offset
    response = await service_client.get('/couriers?limit=5&offset=2')
    assert response.status == 200

    data = response.json()
    assert data['limit'] == 5
    assert data['offset'] == 2
    assert len(data['couriers']) == 3

    cursor = pgsql['lavka'].cursor()
    check_couriers(data['couriers'], cursor)

    response = await service_client.get('/couriers?limit=-1')
    assert response.status == 400
    response = await service_client.get('/couriers?offset=-1')
    assert response.status == 400
    response = await service_client.get('/couriers?offset=123')
    assert response.status == 200
    assert len(response.json()['couriers']) == 0


async def test_post_couriers(service_client, pgsql):
    request_body = {
        "couriers": [
            {
                "courier_type": "AUTO",
                "regions": [54, 154],
                "working_hours": ["10:00-12:00"]
            },
            {
                "courier_type": "BIKE",
                "regions": [42],
                "working_hours": ["13:00-20:00", "09:00-11:00"]
            },
            {
                "courier_type": "FOOT",
                "regions": [98],
                "working_hours": ["09:00-17:00"]
            }
        ]
    }
    response = await service_client.post("/couriers", request_body)
    assert response.status_code == 200
    response_body = response.json()

    assert len(request_body["couriers"]) == len(response_body["couriers"])

    cursor = pgsql['lavka'].cursor()
    for courier_json in request_body["couriers"]:
        cursor.execute("""
                SELECT id FROM lavka.couriers
                WHERE type = '{}' AND regions = '{}' AND working_hours = {};
                """.format(courier_json['courier_type'],
                           "{" + ", ".join(str(x)
                                           for x in courier_json['regions']) + "}",
                           format_hours(courier_json['working_hours'])))
        res = cursor.fetchall()
        assert len(res) == 1

        courier_json["courier_id"] = res[0][0]
        assert courier_json in response_body["couriers"]


async def test_transactions(service_client, pgsql):
    request_body = {
        "couriers": [
            {
                "courier_type": "AUTO",
                "regions": [54, 154],
                "working_hours": ["10:00-12:00"]
            },
            {
                "courier_type": "BIKE",
                "regions": [42],
                "working_hours": ["13:00-20:00", "09:00-11:00"]
            },
            {
                "courier_type": "FOOTBALL",
                "regions": [98],
                "working_hours": ["09:00-17:00"]
            }
        ]
    }
    response = await service_client.post("/couriers", request_body)
    assert response.status_code == 400

    cursor = pgsql['lavka'].cursor()
    cursor.execute("SELECT * FROM lavka.couriers;")
    res = cursor.fetchall()
    assert len(res) == 0


async def test_bad_request(service_client):
    # empty request test
    response = await service_client.post("/couriers")
    assert response.status_code == 400

    # missing path test
    response = await service_client.post("/couriers", {
        "courier_type": "AUTO",
        "regions": [54, 154],
        "working_hours": ["10:00-12:00"]
    })
    assert response.status_code == 400

    # missing field test
    request_body = {
        "couriers": [
            {
                "courier_type": "AUTO",
                "regions": [54, 154],
                "working_hours": ["10:00-12:00"]
            },
            {
                "courier_type": "BIKE",
                "regions": [42],
            }
        ]
    }
    response = await service_client.post("/couriers", request_body)
    assert response.status_code == 400
