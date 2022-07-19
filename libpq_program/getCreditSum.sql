SELECT
  subject_detail.subject_name
  ,subject_detail.necessary
FROM
  subject_detail
  LEFT OUTER JOIN (
    SELECT
      subject_grade.subject_code
      ,subject_grade.opening_year
      ,subject_grade.id
      ,CONCAT(
        SUBSTRING(subject_grade.id, 1, 3),
        SUBSTRING(subject_grade.id, 5, 1)
      ) AS available
      ,subject_grade.grade_point
    FROM
      subject_grade
    WHERE
      subject_grade.id = 'B1120009'
  ) AS grade ON subject_detail.subject_code = grade.subject_code
  AND subject_detail.opening_year = grade.opening_year
  AND subject_detail.courses_available = grade.available
WHERE
  subject_detail.courses_available = 'B110'
  AND subject_detail.classification = 61
  AND grade.grade_point IS NULL
GROUP BY
  subject_detail.subject_name
  ,subject_detail.necessary
ORDER BY
  subject_detail.necessary ASC;