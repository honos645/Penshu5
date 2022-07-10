--
-- grade_judge table
--

CREATE TABLE grade_judge(
id varchar(8),     -- 学籍番号
gpa float,      --GPA
grade_ranking integer,   --席次
promotion_judge integer,   --進級判定
research_judge integer,    --卒検判定
graduation_judge integer,   --卒業判定
completion_judge integer,   --修了判定

PRIMARY KEY (id)
);
