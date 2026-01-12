import pytest


@pytest.mark.pgsql("lavka", files=["init.sql"])
async def test_order_completion(service_client, pgsql):
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
                "courier_id": couriers_ids[2][0],
                "order_id": orders_ids[2][0]
            }
        ]
    }

    response = await service_client.post("/orders/complete", request_json)
    assert response.status_code == 200

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
