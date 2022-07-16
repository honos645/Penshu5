--
-- subject_classification_advance table
--
CREATE TABLE subject_classification_advance(
        classification   integer,     -- 科目区分
        required_credit integer,   -- 　必要な単位数
        absolute_option  integer,   --必修か選択か
        PRIMARY KEY (classification)
);
