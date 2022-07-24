--
-- work table
--
CREATE TABLE work(
      id varchar(10),   --学籍番号
      route varchar(30), --進路区分、ID
      route_industry varchar(30), --産業区分、ID
      route_occupation_id varchar(30), --職業区分,ID
      Employment_name varchar(30), --就職先名
      Employment_subject varchar(30), --就職先所在地件名
      PRIMARY KEY (id)
      );
