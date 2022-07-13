--
-- user table
--
CREATE TABLE users(
        rogin_id   varchar(5),     -- ログインID
        password   integer,   -- 　パスワード
        id  varchar(8),   -- 学籍番号
	person_name varchar(100), -- 人の名前
	adress_code varchar(255), -- 住所
	email_adress varchar(100), --　email
	birthday_date date, -- 生年月日
	user_level integer, -- ユーザレベル
	roll integer, --　役職
	department integer, --　学部
	major integer, --　学科、プログラム
	school_year integer, --　学年
        PRIMARY KEY (rogin_id, id)
);
