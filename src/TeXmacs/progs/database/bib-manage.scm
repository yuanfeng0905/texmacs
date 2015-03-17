
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; MODULE      : bib-manage.scm
;; DESCRIPTION : global bibliography management
;; COPYRIGHT   : (C) 2015  Joris van der Hoeven
;;
;; This software falls under the GNU general public license version 3 or later.
;; It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
;; in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(texmacs-module (database bib-manage)
  (:use (database bib-db)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Default bibliographic database
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (default-bib-db)
  (url->system (string->url "$TEXMACS_HOME_PATH/database/bib.tmdb")))

(define-preferences
  ("bib-db" (default-bib-db) noop))

(tm-define (get-bib-db)
  (get-preference "bib-db"))

(tm-define (set-bib-db val)
  (when (string? val)
    (set-preference "bib-db" val)
    (refresh-now "bib-db-preference")))

(tm-define (get-bib-db-short)
  (with full (system->url (get-bib-db))
    (url->system (url-tail full))))

(tm-define (set-bib-db-short val)
  (when (string? val)
    (with full (system->url (get-bib-db))
      (set-bib-db (url->system (url-relative full (system->url val)))))))

(tm-define (url-bib-db)
  (system->url (get-bib-db)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Caching existing BibTeX files
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define bib-dir "$TEXMACS_HOME_PATH/system/database")
(define bib-master (url->url (string-append bib-dir "/bib-master.tmdb")))

(define (bib-cache-id f)
  (with-database bib-master
    (let* ((s (url->system f))
           (l (db-search (list (list "source" s)))))
      (and (== (length l) 1) (car l)))))

(define (bib-cache-stamp f)
  (and-with id (bib-cache-id f)
    (with-database bib-master
      (db-get-first id "stamp" #f))))

(define (bib-cache-db f)
  (and-with id (bib-cache-id f)
    (with-database bib-master
      (system->url (db-get-first id "target" #f)))))

(define (bib-cache-up-to-date? f)
  (and-with stamp (bib-cache-stamp f)
    (and (url-exists? f)
         (== (number->string (url-last-modified f)) stamp))))

(define (bib-cache-remove f)
  (and-with id (bib-cache-id f)
    (and-with db (bib-cache-db f)
      (system-remove db)
      (with-database bib-master
        (db-reset-all id)))))

(define (bib-cache-create f)
  (let* ((bib-doc (string-load f))
         (tm-doc (convert bib-doc "bibtex-document" "texmacs-stree"))
         (body (tmfile-extract tm-doc 'body))
         (id (create-unique-id))
         (db (url->url (string-append bib-dir "/" id ".tmdb"))))
    (when body
      (with-database db
        (bib-save body))
      (when (url-exists? db)
        (with-database bib-master
          (db-insert id "source" (url->system f))
          (db-insert id "target" (url->system db))
          (db-insert id "stamp" (number->string (url-last-modified f))))))))

(tm-define (bib-cache-bibtex f)
  (when (not (bib-cache-up-to-date? f))
    (bib-cache-remove f))
  (when (not (bib-cache-id f))
    (bib-cache-create f))
  (when (not (bib-cache-id f))
    (texmacs-error "failed to create bibliographic database"
                   "bib-cache-bibtex"))
  (and-with id (bib-cache-id f)
    (url->url (string-append bib-dir "/" id ".tmdb"))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Importing and exporting BibTeX files
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (bib-cache-imported? f bdb)
  (and-with id (bib-cache-id f)
    (and-with db (bib-cache-db f)
      (with-database db
        (== (db-get-first id "imported" #f) (url->system bdb))))))

(define (bib-cache-notify-imported f bdb)
  (and-with id (bib-cache-id f)
    (and-with db (bib-cache-db f)
      (with-database db
        (db-set id "imported" (list (url->system bdb)))))))

(tm-define (bib-import-bibtex f)
  (with db (bib-cache-bibtex f)
    (when (url-exists? db)
      (with-database db
        (with all (bib-load)
          (when (not (bib-cache-imported? f (url-bib-db)))
            (with-database (url-bib-db)
              (bib-save all)
              (bib-cache-notify-imported f (url-bib-db))
              (set-message "Imported bibliographic entries"
                           "import bibliography"))))))))

(tm-define (bib-export-global f)
  (with-database (url-bib-db)
    (with all (bib-load)
      (when (and all (tm-func? all 'document))
        (let* ((doc `(document ,@(map db->bib (cdr all))))
               (bibtex-doc (convert doc "texmacs-stree" "bibtex-document")))
          (string-save bibtex-doc f))))))

(define (bib-entry? t)
  (or (tm-func? t 'bib-entry 3)
      (and (tm-func? t 'db-entry 4)
           (tm-atomic? (tm-ref t 1))
           (in? (tm->string (tm-ref t 1)) bib-types-list))))

(tm-define (bib-export-buffer f)
  (when (tm-func? (buffer-tree) 'document)
    (let* ((l1 (list-filter (tm-children (buffer-tree)) bib-entry?))
           (l2 (map tm->stree l1))
           (l3 (map (lambda (x)
                      (if (tm-func? x 'bib-entry 3) x (db->bib x))) l2))
           (doc `(document ,@l3))
           (bibtex-doc (convert doc "texmacs-stree" "bibtex-document")))
      (string-save bibtex-doc f)
      (set-message "Exported bibliographic entries" "export bibliography"))))

(tm-define (bib-exportable?)
  (or (nnull? (bib-attachments))
      (and (tm-func? (buffer-tree) 'document)
           (list-or (map bib-entry? (tm-children (buffer-tree)))))))

(tm-define (bib-export-bibtex f)
  (if (nnull? (bib-attachments))
      (bib-export-attachments f)
      (bib-export-buffer f)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Retrieving entries
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (bib-retrieve-one name)
  (and-with l (db-search (list (list "name" name)))
    (and (nnull? l)
         (with e (db-load-entry (car l))
           (cons name e)))))

(define (bib-retrieve-several names)
  (if (null? names) (list)
      (let* ((head (bib-retrieve-one (car names)))
             (tail (bib-retrieve-several (cdr names))))
        (if head (cons head tail) tail))))

(define (bib-retrieve-entries-from-one names db)
  (with-database db
    (bib-retrieve-several names)))

(define (bib-retrieve-entries-from names dbs)
  (if (null? dbs) (list)
      (let* ((r (bib-retrieve-entries-from-one names (car dbs)))
             (done (map car r))
             (remaining (list-difference names done)))
        (append r (bib-retrieve-entries-from remaining (cdr dbs))))))

(define (bib-get-db bib-file)
  (cond ((== bib-file :default) (url-bib-db))
        ((== (url-suffix bib-file) "tmdb") (url->url bib-file))
        (else (bib-cache-bibtex bib-file))))

(tm-define (bib-retrieve-entries names . bib-files)
  (set! names (list-remove-duplicates names))
  (with l (list-filter (map bib-get-db bib-files) (lambda (ok?) ok?))
    (bib-retrieve-entries-from names l)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Running bibtex or its internal replacement
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(tm-define (bib-generate prefix style doc)
  (with m `(bibtex ,(string->symbol style))
    (module-provide m)
    (bib-process prefix style doc)))

(define (bib-difference l1 l2)
  (with t (list->ahash-set (map car l2))
    (list-filter l1 (lambda (x) (not (ahash-ref t (car x)))))))

(define (bib-compile-sub prefix style names . bib-files)
  (set! names (list-remove-duplicates names))
  (let* ((all-files (rcons bib-files :default))
         (l (apply bib-retrieve-entries (cons names all-files)))
         (bl (map db->bib (map cdr l)))
         (doc `(document ,@bl)))
    (if (in? style (list "tm-abbrv" "tm-acm" "tm-alpha" "tm-elsartnum"
                         "tm-ieeetr" "tm-plain" "tm-siam"))
        (bib-generate prefix (string-drop style 3) doc)
        (let* ((bib-files*
                (list-filter all-files
                             (lambda (f) (and (url? f)
                                              (== (url-suffix f) "bib")))))
               (l* (apply bib-retrieve-entries (cons names bib-files*)))
               (bl* (map db->bib (map cdr (bib-difference l l*))))
               (doc* `(document ,@bl*))
               (bib-docs (map string-load bib-files*))
               (xdoc (convert doc* "texmacs-stree" "bibtex-document"))
               (all-docs (append bib-docs (list "\n") (list xdoc)))
               (full-doc (apply string-append all-docs))
               (auto (url->url "$TEXMACS_HOME_PATH/system/bib/auto.bib")))
          ;;(display* auto "\n-----------------------------\n" full-doc "\n")
          (string-save full-doc auto)
          (bibtex-run prefix style auto names)))))

(tm-define (bib-compile prefix style names . bib-files)
  (when (and (tm? names) (tm-func? names 'document))
    (set! names (tm-children (tm->stree names))))
  ;;(display* "Compile " style ", " names ", " bib-files "\n")
  (if (not (and (list? names) (list-and (map string? names))))
      (tree "Error: invalid bibliographic key list")
      (with t (apply bib-compile-sub (cons* prefix style names bib-files))
        (if (not (tm? t))
            (tree "Error: failed to produce bibliography")
            (tm->tree t)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Attaching the bibliography to the current document and automatic importation
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(tm-define (bib-attach prefix names . bib-files)
  (when (and (tm? names) (tm-func? names 'document))
    (set! names (tm-children (tm->stree names))))
  (when (and (list? names) (list-and (map string? names)))
    (set! names (list-remove-duplicates names))
    (let* ((all-files (rcons bib-files :default))
           (l (apply bib-retrieve-entries (cons names all-files)))
           (doc `(document ,@(map cdr l))))
      (set-attachment (string-append prefix "-bibliography") doc))))

(define (bib-attachments)
  (with l (list-attachments)
    (list-filter l (cut string-ends? <> "-bibliography"))))

(tm-define (bib-export-attachments f)
  (let* ((l (bib-attachments))
         (bibs (map tm->stree (map get-attachment l)))
         (b (append-map tm-children bibs))
         (doc `(document ,@(map db->bib b)))
         (bibtex-doc (convert doc "texmacs-stree" "bibtex-document")))
    (string-save bibtex-doc f)
    (set-message "Exported bibliographic references" "export bibliography")))

(tm-define (notify-set-attachment name key val)
  (when (string-ends? key "-bibliography")
    (with doc (tm->stree val)
      (with-database (url-bib-db)
        (with-global db-duplicate-warning? #f
          (bib-save doc)))))
  (former name key val))