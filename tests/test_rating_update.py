import pytest
import pytest_userver
import testsuite


@pytest.mark.pgsql("lavka", files=["init.sql"])
async def simple_test(service_client: pytest_userver.client.Client, pgsql):
    cursor = pgsql["lavka"].cursor()

    await service_client.run_periodic_task("update-ratings")

    cursor.execute("SELECT rating FROM lavka.couriers WHERE id = 2")
    assert cursor.fetchone()[0] is None

    cursor.execute("SELECT id FROM lavka.couriers WHERE rating IS NULL LIMIT 1")
    courier_id = cursor.fetchone()[0]

    cursor.executemany("""
        INSERT INTO lavka.orders (weight, regions, delivery_hours, cost, completed_courier_id, completed_time, rating)
        VALUES (%s, %s, %s, %s, %s, %s, %s)
    """, [(5.5, 1, '{"10:00-12:00"}', 500, courier_id, '2025-01-08 23:20:00', '5') for _ in range(10)])
    assert cursor.rowcount == 10

    await service_client.run_periodic_task("update-ratings")

    cursor.execute("SELECT id FROM lavka.couriers WHERE id = {} AND rating = '5'".format(courier_id))
    assert len(cursor.fetchall()) == 1


@pytest.mark.pgsql("lavka", files=["init2.sql"])
async def test_rating_calculation(service_client, pgsql):
    await service_client.run_periodic_task("update-ratings")

    cursor = pgsql["lavka"].cursor()
    cursor.execute("SELECT rating FROM lavka.couriers WHERE id = 1")
    assert cursor.fetchone()[0] == 4