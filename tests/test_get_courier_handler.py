import pytest


@pytest.mark.pgsql("lavka", files=["init.sql"])
async def test_get_courier(service_client, pgsql):
    cursor = pgsql["lavka"].cursor()

    cursor.execute(
        "SELECT * FROM lavka.couriers LIMIT 1;")
    res = cursor.fetchone()

    response = await service_client.get("/couriers/" + str(res[0]))
    assert response.status_code == 200
    assert response.json() == {
        "courier_id": res[0],
        "courier_type": res[1],
        "regions": res[2],
        "working_hours": res[3]
    }

    cursor.execute(
        "SELECT * FROM lavka.couriers LIMIT 1 OFFSET 3;")
    res = cursor.fetchone()

    response = await service_client.get("/couriers/" + str(res[0]))
    assert response.status_code == 200
    assert response.json() == {
        "courier_id": res[0],
        "courier_type": res[1],
        "regions": res[2],
        "working_hours": res[3]
    }

    # invalid id
    id = 5252
    cursor.execute(
        f"SELECT * FROM lavka.couriers WHERE id = {id};")
    assert cursor.rowcount == 0

    response = await service_client.get("/couriers/" + str(id))
    assert response.status_code == 400
