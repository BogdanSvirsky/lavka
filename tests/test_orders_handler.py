import pytest

from test_couriers_handler import format_hours
from datetime import datetime, timezone


def compare_datetime(a: datetime, b: datetime):
    return a.astimezone(tz=timezone.utc) == b.astimezone(tz=timezone.utc)


@pytest.mark.pgsql("lavka", files=["init.sql"])
async def test_get_orders(service_client, pgsql):
    def check_orders(orders_json, cursor):
        for order_json in orders_json:
            cursor.execute("""
                    SELECT id, completed_time FROM lavka.orders
                    WHERE id = {} AND weight = {} AND regions = {} AND delivery_hours = {} AND cost = {};
                    """.format(str(order_json['order_id']), order_json['weight'], order_json['regions'],
                               format_hours(
                                   order_json['delivery_hours']), order_json['cost']))
            res = cursor.fetchall()
            assert len(res) == 1
            if 'completed_time' in order_json.keys():
                assert compare_datetime(datetime.fromisoformat(
                    order_json["completed_time"]), res[0][1])

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

    data = response.json()
    assert len(data) == 7

    cursor = pgsql['lavka'].cursor()
    check_orders(data, cursor)

    # bad request
    response = await service_client.get('/couriers?limit=-1')
    assert response.status == 400
    response = await service_client.get('/couriers?offset=-1')
    assert response.status == 400
    response = await service_client.get('/couriers?offset=123')
    assert response.status == 200
    assert len(response.json()['couriers']) == 0


async def test_post_orders(service_client, pgsql):
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
