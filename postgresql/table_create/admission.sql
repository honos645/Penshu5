--
-- admission table
--
CREATE TABLE admission (
    id  varchar(8),  --学籍番号
    Pathway_classfication varchar(100),  --進路区分
    Highereducation_name varchar(100),  --進学先名
    Highereducation_place varchar(100),  --進学先所在地
    PRIMARY KEY(id)
);
