-- 取得した科目区分ごとの取得単位数を表示
-- 67200072_宇佐見
SELECT
  grade_list.classification_name AS name
  ,grade_list.classification
  ,sum(
    CASE
      WHEN
        grade_list.grade_point >= 60 AND grade_list.grade_point <= 100
      THEN
        grade_list.credit
      ELSE
        0
    END
    ) AS total
  FROM
  (
    SELECT
      subject_grade.id
      ,subject_detail.subject_code
      ,subject_detail.course_sharing_code
      ,subject_detail.subject_name
      ,subject_detail.credit
      ,subject_detail.classification
      ,classification_name.classification_name
      ,subject_detail.courses_available
      ,subject_grade.grade_point
    FROM
      (
        subject_detail
        INNER JOIN
          classification_name
          ON
          subject_detail.classification = classification_name.classification_code
      )
      LEFT OUTER JOIN
        subject_grade
        ON
        subject_detail.subject_code = subject_grade.subject_code
      AND
        subject_detail.opening_year = subject_grade.opening_year
    WHERE
      subject_detail.courses_available = 'B110'
      AND
      (
        subject_grade.id = 'B1120001'
        OR
        subject_grade.id IS NULL
      )
  ) AS grade_list
GROUP BY
  grade_list.classification
  ,grade_list.classification_name
;
