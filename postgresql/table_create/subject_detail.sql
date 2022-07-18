--
-- subjectdetail_t table
--
CREATE TABLE subject_detail(
    subject_code integer,  -- 科目コード
    opening_year integer,  -- 開講年度
    subject_name varchar(255),  --科目名
    course_sharing_code integer,  --科目共有コード
    credit integer,  --単位数
    courses_available varchar(5), --履修可能学科
    classification integer,  -- 科目区分
    necessary integer,  -- 必修選択区分
    PRIMARY KEY (subject_code, opening_year)
);
