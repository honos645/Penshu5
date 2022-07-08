--
-- route_occupation table
--
CREATE TABLE route_occupation(
    occupation_id varchar(16) UNIQUE,  --職業区分ID
    occupation_name varchar(128) UNIQUE,  --職業区分名
    PRIMARY KEY(occupation_name)
);
