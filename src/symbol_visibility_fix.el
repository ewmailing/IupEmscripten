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
          (print (thing-at-point 'line))
          (print (preceding-char))
          (print (thing-at-point 'char))
          (print (thing-at-point 'word))
          (forward-char -18)
          (print (thing-at-point 'word))
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

;; so i'm using (insert-file-contents) to copy a C file into a temp
;; buffer.  i'm then trying to traverse the file.  i use
;; (search-forward) to get to the function i need, but my last step is
;; inserting something at the beginning of the function.  i have: int
;; myfunction(int arg1, int arg2).  the point it at the opening (. However, whenever I try to move back a word or move back a char, i can only move back as far as the 'm' in myfunction. whenever i try to go past that whitespace between int and myfunction, emacs freezes.  why is this?
