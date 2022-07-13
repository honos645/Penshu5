-- 講義リストに学生の成績を付与
-- 67200072_宇佐見
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
ORDER BY
  subject_detail.classification ASC
;