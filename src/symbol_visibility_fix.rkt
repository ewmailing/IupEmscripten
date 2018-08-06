; Symbol Visibility Fix
; Author: Chris Matzenbach
#lang racket

;; global vars
(define inputDir ".") 

;; get function list from .def files
(define (get-function-list fle)
  (file->lines fle))

;; removes any empty strings in a list
(define (remove-empty-strings-from-list lst)
  (filter (lambda (x) (not (equal? "" x))) lst))

;; search through file for matching string - case sensitive
;; returns the string if match is found, false otherwise
(define (find-string-in-file fle str)
  (call-with-input-file fle
    (lambda (in) (let ([res (string-contains? (port->string in) str)])
                   (if (eq? res #t) str #f)))))

(define (launch defFile dir)
  (let ([funcList (remove-empty-strings-from-list (get-function-list defFile))])
    ;; open every file in folder and check for match in func list
    (for ([fle (in-directory dir)] #:when (or (file-exists? fle) (not (string-contains? (file->string fle) ".cquery_cached_index"))))
      (for/list ([str funcList])
        (find-string-in-file fle str)))))

;; (define (launch defFile dir)
;;   (let ([funcList (remove-empty-strings-from-list (get-function-list defFile))])
;;     ;; open every file in folder and check for match in func list
;;     (for ([fle (in-directory dir)] #:when (file-exists? fle))
;;       (for/list ([str funcList])
;;         (if (not (eq? (find-string-in-file fle str) #f))
;;             (find-string-in-file fle str)
;;             '(""))))))

;; ;; main function
;; (defun sym-vis-fix (searchFunc)

;;   (defun my-process-file (fPath)
;;     "Process the file at FPATH …"
;;     (let (myBuffer p1 p2 (ii 0) searchStr)
;;       (when (not (string-match "/xx" fPath)) ; exclude some dir
;;         (let ((fileChanged-p nil))
;;           (with-temp-buffer
;;             ;; insert code from file into temp buffer
;;             (insert-file-contents fPath nil nil nil t)

;;             ;; need to get all search keys in here and iterate through each
;;             (setq searchStr searchFunc) ; search string here
;;             ;; start from first point in buffer
;;             (goto-char 1)
;;             (while (search-forward searchStr nil t)
;;               ;; go to beginning of function def line
;;               (forward-word -3)
;;               (if (not (string-equal (thing-at-point 'word) "IUP_EXPORTI"))
;;                 (forward-word 1)
;;                 (insert "IUP_EXPORTI "))
;;               ;; go back to cursor start
;;               (forward-word 3)
;;               (setq fileChanged-p t)
;;               ;; (setq ii (1+ ii))
;;               )
;;             (when fileChanged-p (write-region 1 (point-max) fPath)))))))

;;   (let (outputBuffer)
;;     (setq outputBuffer "*my occur output*" )
;;     (with-output-to-temp-buffer outputBuffer
;;       (mapc 'my-process-file (find-lisp-find-files inputDir "\\.h$"))
;;       (princ "Done deal!"))))

;; ;; apply main function to the function list to get everything started
;; (mapc 'sym-vis-fix searchList)