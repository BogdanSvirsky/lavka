CREATE TABLE IF NOT EXISTS orders (
    id BIGSERIAL PRIMARY KEY,
    weight DECIMAL NOT NULL,
    regions DECIMAL NOT NULL,
    delivery_hours VARCHAR(11)[] NOT NULL,
    cost INTEGER NOT NULL,
    completed_time TIMESTAMP NOT NULL
);

CREATE TYPE courier_type AS ENUM ('FOOT', 'BIKE', 'AUTO');
CREATE TABLE IF NOT EXISTS couriers (
    id BIGSERIAL PRIMARY KEY,
    type courier_type NOT NULL,
    regions integer[] NOT NULL,
    working_hours VARCHAR(11)[] NOT NULL
);