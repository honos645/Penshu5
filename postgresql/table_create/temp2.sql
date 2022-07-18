SELECT
  subject_grade.id
  ,subject_detail.subject_code
  ,subject_detail.course_sharing_code
  ,subject_detail.subject_name
  ,subject_detail.credit
  ,subject_detail.classification
  ,subject_detail.courses_available
  ,subject_grade.grade_point
FROM
  subject_detail
  LEFT OUTER JOIN
    subject_grade
  ON
    subject_detail.course_sharing_code = subject_grade.subject_code
    -- ,subject_detail.opening_year = subject_grade.opening_year;
WHERE
  subject_grade.id IN('B1120001', NULL)
  AND subject_detail.courses_available = 'B110';