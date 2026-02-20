import pytest
from datetime import datetime, timezone

from test_orders_handler import validate_orders


@pytest.mark.pgsql("lavka", files=["init.sql"])
async def test_get_order(service_client, pgsql):
    cursor = pgsql['lavka'].cursor()

    cursor.execute(
        "SELECT * FROM lavka.orders WHERE completed_time IS NULL LIMIT 1;")
    order = cursor.fetchone()
    response = await service_client.get("/orders/" + str(order[0]))
    assert response.status_code == 200
    response_json = response.json()
    validate_orders(response_json, order)

    cursor.execute(
        "SELECT * FROM lavka.orders WHERE completed_time IS NOT NULL LIMIT 1;")
    order = cursor.fetchone()
    response = await service_client.get("/orders/" + str(order[0]))
    assert response.status_code == 200
    response_json = response.json()
    validate_orders(response_json, order)

    response = await service_client.get("/orders/93737")
    assert response.status_code == 400
