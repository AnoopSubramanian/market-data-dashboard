CREATE TABLE instrument (
    id TEXT PRIMARY KEY,
    exchange TEXT NOT NULL,
    symbol TEXT NOT NULL,
    instrument_type TEXT NOT NULL
);

CREATE TABLE market_data (
    id SERIAL PRIMARY KEY,
    instrument_id TEXT NOT NULL REFERENCES instrument(id),
    ts TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    price DOUBLE PRECISION,
    bid DOUBLE PRECISION,
    ask DOUBLE PRECISION,
    volume_24h DOUBLE PRECISION
);
