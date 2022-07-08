--
-- user table
--
CREATE TABLE users(
    rogin_id varchar(5),  -- ログインID
    password integer,  -- 　パスワード
    id varchar(8),  -- 学籍番号
    person_name varchar(100),  -- 人の名前
    adress_code varchar(255),  -- 住所
    email_adress varchar(100),  -- email
    birthday_date date,  -- 生年月日
    user_level varchar(1),  -- ユーザレベル
    undergraduate_name varchar(1),  -- 学部
    department_name varchar(1),  -- 学科
    mayjor_name varchar(1),  -- プログラム
    school_year integer,  -- 学年
    PRIMARY KEY(rogin_id, id)
);
