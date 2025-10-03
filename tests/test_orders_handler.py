async def test_hello_base(service_client):
    response = await service_client.get('/orders')
    assert response.status == 200
    response = await service_client.post('/orders')
    assert response.status == 200
