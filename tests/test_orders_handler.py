import pytest

from test_couriers_handler import format_hours
from datetime import datetime, timezone
from dateutil import parser


def compare_datetime(a: datetime, b: datetime):
    return a.astimezone(tz=timezone.utc) == b.astimezone(tz=timezone.utc)

def validate_orders(order_json, order_row):
    # may not be equivalent due to inaccuracy
    assert order_row[1] == order_json['weight']
    assert order_row[2] == order_json['regions']
    assert order_row[3] == order_json['delivery_hours']
    assert order_row[4] == order_json['cost']
    if order_row[7] is not None:
        assert order_row[5] is not None
        assert compare_datetime(parser.isoparse(
            order_json["completed_time"]), order_row[7])
        if order_row[6] is not None:
            assert order_row[6] == str(order_json["rating"])
        else:
            assert "rating" not in order_json.keys()
    else:
        assert order_row[5] is None and "completed_time" not in order_json.keys()

@pytest.mark.pgsql("lavka", files=["init.sql"])
async def test_get(service_client, pgsql):
    def check_orders(orders_json, cursor):
        for order_json in orders_json:
            cursor.execute(
                "SELECT * FROM lavka.orders WHERE id = {};".format(str(order_json["order_id"])))
            res = cursor.fetchone()

            validate_orders(order_json, res)

    # all presented
    response = await service_client.get('/orders?limit=10')
    assert response.status == 200

    data = response.json()
    assert len(data) == 10

    cursor = pgsql['lavka'].cursor()
    check_orders(data, cursor)

    # offset
    response = await service_client.get('/orders?limit=10&offset=3')
    assert response.status == 200

    cursor.execute("SELECT COUNT(*) FROM lavka.orders")
    assert len(response.json()) == min(cursor.fetchone()[0] - 3, 10)

    cursor = pgsql['lavka'].cursor()
    check_orders(data, cursor)


async def test_bad_get_request(service_client):
    response = await service_client.get('/orders?limit=-1')
    assert response.status == 400
    response = await service_client.get('/orders?offset=-1')
    assert response.status == 400
    response = await service_client.get('/orders?offset=123')
    assert response.status == 200
    assert len(response.json()) == 0


async def test_creation(service_client, pgsql):
    # correct request test
    request_body = {
        "orders": [
            {
                "weight": 1,
                "regions": 2,
                "cost": 3,
                "delivery_hours": ["08:00-09:00"]
            },
            {
                "weight": 4,
                "regions": 5,
                "cost": 6,
                "delivery_hours": ["10:00-12:00"]
            },
            {
                "weight": 7,
                "regions": 8,
                "cost": 9,
                "delivery_hours": ["15:00-16:00"]
            }
        ]
    }
    response = await service_client.post("/orders", request_body)
    assert response.status_code == 200
    response_body = response.json()

    cursor = pgsql['lavka'].cursor()
    for order_json in request_body["orders"]:
        cursor.execute(
            """
            SELECT id FROM lavka.orders 
            WHERE weight = {} AND regions = {} AND cost = {} AND delivery_hours = {} 
        """.format(order_json["weight"], order_json["regions"], order_json["cost"], format_hours(order_json["delivery_hours"])))
        res = cursor.fetchall()
        assert len(res) == 1
        order_json["order_id"] = res[0][0]
        assert order_json in response_body


async def test_bad_create_request(service_client):
    # empty request test
    response = await service_client.post("/orders")
    assert response.status_code == 400

    # invalid path test
    response = await service_client.post("/orders", {
        "weight": 7,
        "regions": 8,
        "cost": 9,
        "delivery_hours": ["15:00-16:00"]
    })
    assert response.status_code == 400

    # missing fields test
    request_body = {
        "orders": [
            {
                "weight": 1,
                "regions": 2,
                "cost": 3,
                "delivery_hours": ["08:00-09:00"]
            },
            {
                "weight": 4,
                "cost": 6,
                "delivery_hours": ["10:00-12:00"]
            }
        ]
    }
    response = await service_client.post("/orders", request_body)
    assert response.status_code == 400
