
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; MODULE      : tmweb.scm
;; DESCRIPTION : automatic generation of web sites
;; COPYRIGHT   : (C) 2003  Joris van der Hoeven
;;
;; This software falls under the GNU general public license version 3 or later.
;; It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
;; in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(texmacs-module (doc tmweb)
  (:use (texmacs texmacs tm-files)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Building a web site
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (tmweb-make-dir dir html-dir)
  (when (and (!= dir html-dir) (!= dir (system->url ".")))
    (tmweb-make-dir (url-expand (url-append dir (url-parent))) html-dir))
  (when (not (url-exists? dir))
    (display* "TeXmacs] Creating directory " (url->system dir) "\n")
    (system-mkdir dir)
    (system-1 "chmod a+x" dir)))

(define (tmweb-convert-file tm-file html-file)
  (with-aux tm-file
    (if (url? html-file) (set! current-save-target html-file))
    (export-buffer-main (current-buffer) html-file "html" (list :overwrite))))

(define (needs-update? src dest update?)
  (or (not update?)
      (not (url-exists? dest))
      (url-newer? src dest)))

(define (tmweb-convert-file-dir file tm-dir html-dir update?)
  (let* ((m? (== (get-preference "texmacs->html:mathml") "on"))
	 (u1 (url-delta (url-append tm-dir "dummy") file))
	 (u2 (url-glue (url-unglue u1 2) (if m? "xhtml" "html")))
	 (u3 (url-append html-dir u2))
	 (dir (url-expand (url-append u3 (url-parent))))
	 (dir-name (url->system (url-tail dir))))
    (when (and (!= dir-name "CVS") (!= dir-name ".svn")
	       (!= dir-name "prop-base") (!= dir-name "text-base"))
      (tmweb-make-dir dir (url-expand html-dir))
      (when (needs-update? file u3 update?)
        (system-wait "Converting" (url->system u1))
        (display* "TeXmacs] Converting " (url->system u1) "\n")
        (tmweb-convert-file file u3)))))

(define (tmweb-copy-file-dir file tm-dir html-dir update?)
  (let* ((u1 (url-delta (url-append tm-dir "dummy") file))
	 (u2 (url-append html-dir u1))
	 (name (url->system (url-tail u2)))
	 (dir (url-expand (url-append u2 (url-parent))))
	 (dir-name (url->system (url-tail dir))))
    (when (and (!= dir-name "CVS") (!= dir-name ".svn")
	       (!= dir-name "prop-base") (!= dir-name "text-base")
	       (not (string-ends? name "~"))
               (not (string-ends? name "#")))
      (tmweb-make-dir dir (url-expand html-dir))
      (when (needs-update? file u2 update?)
        (system-wait "Copying" (url->system u1))
        (display* "TeXmacs] Copying " (url->system u1) "\n")
        (system-copy file u2)))))

(define (tmweb-convert-directory tm-dir html-dir update? keep?)
  (let* ((u1 (url-append tm-dir (url-any)))
	 (u2 (url-expand (url-complete u1 "dr")))
	 (u3 (url-append u2 (url-wildcard "*.tm")))
	 (u4 (url-expand (url-complete u3 "fr")))
	 (u5 (url-expand (url-complete u1 "fr"))))
    (when (!= html-dir tm-dir)
      (for-each (lambda (x) (tmweb-copy-file-dir x tm-dir html-dir update?))
                (if keep? (url->list u5)
                    (list-difference (url->list u5) (url->list u4)))))
    (for-each (lambda (x) (tmweb-convert-file-dir x tm-dir html-dir update?))
	      (url->list u4))))

(tm-define (tmweb-convert-dir tm-dir html-dir)
  (tmweb-convert-directory tm-dir html-dir #f #f))

(tm-define (tmweb-update-dir tm-dir html-dir)
  (tmweb-convert-directory tm-dir html-dir #t #f))

(tm-define (tmweb-convert-dir-keep-texmacs tm-dir html-dir)
  (tmweb-convert-directory tm-dir html-dir #f #t))

(tm-define (tmweb-update-dir-keep-texmacs tm-dir html-dir)
  (tmweb-convert-directory tm-dir html-dir #t #t))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Interface
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(tm-define (tmweb-interactive-build)
  (:interactive #t)
  (user-url "Source directory" "directory" 
    (lambda (src)  (user-url "Destination directory" "directory"
      (lambda (dest) (tmweb-convert-directory src dest #f #f))))))

(tm-define (tmweb-interactive-update)
  (:interactive #t)
  (user-url "Source directory" "directory" 
    (lambda (src)  (user-url "Destination directory" "directory"
      (lambda (dest) (tmweb-convert-directory src dest #t #f))))))
