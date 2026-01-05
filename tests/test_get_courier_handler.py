from testsuite.daemons.service_client import Client


async def test_get_courier(service_client: Client):
    response = await service_client.get("/couriers/29")
    assert response.status_code == 200
    assert response.json() == {
        "courier_id": 29,
        "courier_type": "AUTO",
        "regions": [54],
        "working_hours": ["10:00-18:00"]
    }
