SELECT
  classification_name.name
  ,subject_classification_advance.classification
  ,subject_classification_advance.required_credit
  ,CASE
    WHEN credit.sum IS NULL THEN 0
    ELSE credit.sum
  END
  ,subject_classification_advance.absolute_option
FROM
  subject_classification_advance
  LEFT OUTER JOIN (
    SELECT
      subject_detail.classification
      ,SUM(subject_detail.credit) AS SUM
    FROM
      subject_detail
      LEFT OUTER JOIN (
        SELECT
          subject_grade.subject_code
          ,subject_grade.opening_year
          ,subject_grade.id
          ,CONCAT(
            SUBSTRING(subject_grade.id, 1, 3)
            ,SUBSTRING(subject_grade.id, 5, 1)
          ) AS available
          ,subject_grade.grade_point
        FROM
          subject_grade
        WHERE
          subject_grade.id = 'B1120009'
      ) AS grade
      ON subject_detail.subject_code = grade.subject_code
      AND subject_detail.opening_year = grade.opening_year
      AND subject_detail.courses_available = grade.available
    WHERE
      subject_detail.courses_available = 'B110'
      AND (
        grade.grade_point >= 60
        AND grade.grade_point <= 100
      )
    GROUP BY
      subject_detail.classification
    ORDER BY
      subject_detail.classification ASC
  ) AS credit
  ON subject_classification_advance.classification = credit.classification
  INNER JOIN classification_name
  on subject_classification_advance.classification = classification_name.code
;