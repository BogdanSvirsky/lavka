CREATE SCHEMA IF NOT EXISTS lavka;

CREATE TYPE lavka.courier_type AS ENUM ('FOOT', 'BIKE', 'AUTO');
CREATE TABLE IF NOT EXISTS lavka.couriers (
    id BIGSERIAL PRIMARY KEY,
    type lavka.courier_type NOT NULL,
    regions integer[] NOT NULL,
    working_hours VARCHAR(11)[] NOT NULL
);

CREATE TABLE IF NOT EXISTS lavka.orders (
    id BIGSERIAL PRIMARY KEY,
    weight REAL NOT NULL,
    regions INTEGER NOT NULL,
    delivery_hours VARCHAR(11)[] NOT NULL,
    cost INTEGER NOT NULL,
    completed_courier_id BIGINT REFERENCES lavka.couriers(id) DEFAULT null,
    completed_time TIMESTAMP
);