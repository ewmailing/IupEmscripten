;; input dir
(setq inputDir ".") ; In elisp, dir path should end with a slash

(defun my-process-file (fPath)
  "Process the file at FPATH …"
  (let (myBuffer p1 p2 (ii 0) searchStr)
    (when (not (string-match "/xx" fPath)) ; exclude some dir

      (with-temp-buffer
        (insert-file-contents fPath nil nil nil t)

        (setq searchStr "iupdrvSendKey" ) ; search string here

        (goto-char 1)
        (while (search-forward searchStr nil t)
          (setq ii (1+ ii)))

        ;; report if the occurrence is not n times
        (when (not (= ii 0))
          (princ (format "this many: %d %s\n" ii fPath)))))))

;; traverse the dir

(require 'find-lisp)

(let (outputBuffer)
  (setq outputBuffer "*my occur output*" )
  (with-output-to-temp-buffer outputBuffer
    (mapc 'my-process-file (find-lisp-find-files inputDir "\\.h$"))
    (princ "Done deal!")))
