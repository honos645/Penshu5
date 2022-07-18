SELECT
  subject_detail.subject_name
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
    WHERE subject_grade.id = 'B1120009'
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
  subject_detail.classification = 22
  AND
  (
    (
      grade.grade_point <> 666
      AND
      (
        grade.grade_point < 60
        OR
        grade.grade_point > 100
      )
    )
    OR
    grade.grade_point IS NULL
  )
ORDER BY
  subject_detail.classification ASC