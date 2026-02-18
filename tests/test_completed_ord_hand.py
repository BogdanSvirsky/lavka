from datetime import datetime
import pytest

from test_orders_handler import compare_datetime
from dateutil import parser


@pytest.mark.pgsql("lavka", files=["init.sql"])
async def test_order_completion(service_client, pgsql):
    cursor = pgsql['lavka'].cursor()

    # test good request
    cursor.execute(
        "SELECT id FROM lavka.orders WHERE completed_time IS NULL LIMIT 3;")
    orders_ids = cursor.fetchall()

    cursor.execute(
        "SELECT id FROM lavka.couriers LIMIT 3;")
    couriers_ids = cursor.fetchall()

    request_json = {
        "complete_info": [
            {
                "complete_time": "2024-05-09T20:55:59.547Z",
                "courier_id": couriers_ids[0][0],
                "order_id": orders_ids[0][0],
                "rating": 1
            },
            {
                "complete_time": "2025-05-09T20:55:59.547Z",
                "courier_id": couriers_ids[1][0],
                "order_id": orders_ids[1][0],
                "rating": 2
            },
            {
                "complete_time": "2026-05-09T20:55:59.547Z",
                "courier_id": couriers_ids[2][0],
                "order_id": orders_ids[2][0]
            }
        ]
    }

    response = await service_client.post("/orders/complete", request_json)
    assert response.status_code == 200

    for complete_info in request_json["complete_info"]:
        cursor.execute("SELECT completed_time, completed_courier_id, rating FROM lavka.orders WHERE id = {};".format(
            str(complete_info["order_id"])))
        res = cursor.fetchone()

        assert compare_datetime(res[0], parser.isoparse(
            complete_info["complete_time"]))
        assert res[1] == complete_info["courier_id"]
        if "rating" in complete_info.keys():
            assert res[2] == str(complete_info["rating"])


@pytest.mark.pgsql("lavka", files=["init.sql"])
async def test_transactions(service_client, pgsql):
    cursor = pgsql['lavka'].cursor()

    cursor.execute(
        "SELECT id FROM lavka.orders WHERE completed_time IS NULL LIMIT 3;")
    orders_ids = cursor.fetchall()

    cursor.execute(
        "SELECT id FROM lavka.couriers LIMIT 3;")
    couriers_ids = cursor.fetchall()

    request_json = {
        "complete_info": [
            {
                "complete_time": "2024-05-09T20:55:59.547Z",
                "courier_id": couriers_ids[0][0],
                "order_id": orders_ids[0][0]
            },
            {
                "complete_time": "2025-05-09T20:55:59.547Z",
                "courier_id": couriers_ids[1][0],
                "order_id": orders_ids[1][0]
            },
            {
                "complete_time": "2026-05-09T20:55:59.547Z",
                "courier_id": 52525252,
                "order_id": 99999
            }
        ]
    }

    response = await service_client.post("/orders/complete", request_json)
    assert response.status_code == 400

    cursor.execute(
        f"SELECT completed_time FROM lavka.orders \
            WHERE id = {orders_ids[0][0]} OR id = {orders_ids[1][0]} OR id = {orders_ids[2][0]};")
    res = cursor.fetchall()

    for row in res:
        assert row[0] is None


async def test_bad_request(service_client):
    request_json = {
        "complete_time": "2024-05-09T20:55:59.547Z",
        "courier_id": 1,
        "order_id": 3
    }

    response = await service_client.post("/orders/complete", request_json)
    assert response.status_code == 400

    request_json = {
        "complete_info": [
            {
                "complete_time": "2024-05-09T20:55:59.547Z",
                "courier_id": 5252,
                "order_id": 934948
            }
        ]
    }

    response = await service_client.post("/orders/complete", request_json)
    assert response.status_code == 400
