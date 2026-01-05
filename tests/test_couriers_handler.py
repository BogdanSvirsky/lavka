from testsuite.daemons.service_client import Client


async def test_get_couriers(service_client: Client):
    response = await service_client.get('/couriers')
    assert response.status == 200
    assert response.json() == {
        "couriers": [
            {
                "courier_id": 0,
                "courier_type": "BIKE",
                "regions": [54],
                "working_hours": ["09:00-14:00"]
            }
        ],
        "limit": 1,
        "offset": 0
    }

    response = await service_client.get('/couriers?limit=10&offset=132')
    body = response.json()
    assert body["limit"] == 10
    assert body["offset"] == 132


async def test_post_couriers(service_client: Client):
    # correct request test
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

    for i in range(len(response_body["couriers"])):
        request_body["couriers"][i]["courier_id"] = response_body["couriers"][i]["courier_id"]

    assert request_body == response_body

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
