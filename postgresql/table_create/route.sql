--
-- route table
--
CREATE TABLE route(
    route_id integer UNIQUE,  --進路区分ID
    route_name varchar(128) UNIQUE,  --進路区分
    PRIMARY KEY(route_name)
);
