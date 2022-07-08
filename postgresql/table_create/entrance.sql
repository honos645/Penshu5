--
-- entrance table
--
CREATE TABLE entrance(
    id varchar(8),  -- 学籍番号
    exam_classification integer,  -- 入試区分
    score_rate   float,  -- 得点率
    PRIMARY KEY(id)
);
