CREATE TABLE subject_grade (
       subject_code	   integer, --科目コード
       opening_year	   integer,	--開講年度
       id	   	   varchar(8), --学籍番号
       grade_point	   integer,    --成績素点
       retake           integer,    --再履修
       PRIMARY KEY (subject_code, opening_year, id)
);
