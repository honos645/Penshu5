--
-- route_industry table
--

CREATE TABLE route_industry(
    industry_id varchar(16) UNIQUE,  -- 産業区分ID
    industry_name varchar(128) UNIQUE,  -- 産業区分
    PRIMARY KEY(industry_name)
);
