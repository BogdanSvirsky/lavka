from testsuite.daemons.service_client import Client


async def test_get_orders(service_client: Client):
    response = await service_client.get("/orders")
    assert response.status_code == 200
    assert response.json() == [
        {
            "order_id": 0,
            "weight": 0,
            "regions": 0,
            "cost": 0,
            "delivery_hours": ["08:00-09:00"]
        }
    ]


async def test_post_orders(service_client: Client):
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

    for i in range(len(response_body)):
        request_body["orders"][i]["order_id"] = response_body[i]["order_id"]

    assert request_body["orders"] == response_body

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
