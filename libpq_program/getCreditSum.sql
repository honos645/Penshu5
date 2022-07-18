SELECT
  subject_detail.classification
  ,SUM(subject_detail.credit) AS SUM
FROM
  subject_detail
  LEFT OUTER JOIN
  (
    SELECT
      subject_grade.subject_code,
      subject_grade.opening_year,
      subject_grade.id,
      CONCAT(
        SUBSTRING(subject_grade.id, 1, 3),
        SUBSTRING(subject_grade.id, 5, 1)
      ) AS available,
      subject_grade.grade_point
    FROM
      subject_grade
  ) AS grade
  ON
  subject_detail.subject_code = grade.subject_code
  AND
  subject_detail.opening_year = grade.opening_year
  AND
  subject_detail.courses_available = grade.available
WHERE
  subject_detail.courses_available = 'B110'
  AND
  (
    grade.grade_point >= 60
    AND
    grade.grade_point <= 100
  )
GROUP BY
  subject_detail.classification
ORDER BY
  subject_detail.classification ASC
;