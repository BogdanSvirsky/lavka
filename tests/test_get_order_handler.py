import pytest
from datetime import datetime, timezone

from test_orders_handler import compare_datetime


@pytest.mark.pgsql("lavka", files=["init.sql"])
async def test_get_order(service_client, pgsql):
    cursor = pgsql['lavka'].cursor()

    cursor.execute(
        "SELECT * FROM lavka.orders WHERE completed_time IS NULL LIMIT 1;")
    order = cursor.fetchone()
    response = await service_client.get("/orders/" + str(order[0]))
    assert response.status_code == 200
    response_json = response.json()
    assert response_json['order_id'] == order[0]
    assert -0.0001 < response_json['weight'] - order[1] < 0.0001
    assert response_json['regions'] == order[2]
    assert response_json['delivery_hours'] == order[3]
    assert response_json['cost'] == order[4]
    assert ('completed_time' not in response_json.keys()
            or response_json["completed_time"] == None)

    cursor.execute(
        "SELECT id, weight, regions, delivery_hours, cost, completed_time "
        "FROM lavka.orders WHERE completed_time IS NOT NULL LIMIT 1;")
    order = cursor.fetchone()
    response = await service_client.get("/orders/" + str(order[0]))
    assert response.status_code == 200
    response_json = response.json()
    assert response_json['order_id'] == order[0]
    assert -0.0001 < response_json['weight'] - order[1] < 0.0001
    assert response_json['regions'] == order[2]
    assert response_json['delivery_hours'] == order[3]
    assert response_json['cost'] == order[4]
    assert compare_datetime(datetime.fromisoformat(
        response_json["completed_time"]), order[5])

    response = await service_client.get("/orders/93737")
    assert response.status_code == 400
