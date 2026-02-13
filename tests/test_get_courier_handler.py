import pytest

from test_couriers_handler import validate_courier_json


@pytest.mark.pgsql("lavka", files=["init.sql"])
async def test_get_courier(service_client, pgsql):
    cursor = pgsql["lavka"].cursor()

    cursor.execute(
        "SELECT * FROM lavka.couriers LIMIT 1;")
    res = cursor.fetchone()

    response = await service_client.get("/couriers/" + str(res[0]))
    assert response.status_code == 200
    validate_courier_json(response.json(), res)

    cursor.execute(
        "SELECT * FROM lavka.couriers LIMIT 1 OFFSET 3;")
    res = cursor.fetchone()

    response = await service_client.get("/couriers/" + str(res[0]))
    assert response.status_code == 200
    validate_courier_json(response.json(), res)


async def test_invalid_id(service_client):
    response = await service_client.get("/couriers/525252")
    assert response.status_code == 400
