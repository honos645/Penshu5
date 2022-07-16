--
-- subject_classification_graduate table
--
CREATE TABLE subject_classification_graduate(
        classification   integer,     -- 科目区分
        required_credit integer,   -- 　必要な単位数
        absolute_option  integer,   --必修か選択か
        PRIMARY KEY (classification)
);
