from testsuite.daemons.service_client import Client


async def test_get_order(service_client: Client):
    order_id = 52
    response = await service_client.get("/orders/" + str(order_id))
    assert response.status_code == 200
    assert response.json() == {
        "order_id": order_id,
        "weight": 0,
        "regions": 0,
        "cost": 0,
        "delivery_hours": ["08:00-09:00"]
    }
