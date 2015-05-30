(ns BlackJack)

;; !Support part

(def row '(2 3 4 5 6 7 8 9 10 10 10 10 11))
(def deck (list row row row row))

(defn rcard []
  "Get random card from a deck"
  (rand-nth (rand-nth deck)))

;; !Useful part

(defn dealer-plays
  "Simulate dealer's game when he hits another card"
  [sum]
  (cond (> sum 21) false ;; it is fail
        (> sum 17) true  ;; deals doesn't have to hit
        :else (dealer-plays (+ sum
                               (let [r (rcard)]  ;; ace cand be both 11 and 1
                                 (if (and (= r 11) (> sum 10)) 1 r))))))

(defn dealer-starts-with
  "Simulate dealer's game for [acc] times"
  [faced-up wins fails acc]
  (if (> acc 1)
    (if (dealer-plays faced-up)
      (dealer-starts-with faced-up (+ wins 1) fails (- acc 1))  ;; dealer wins
      (dealer-starts-with faced-up wins (+ fails 1) (- acc 1))) ;; dealer fails
    (float (/ wins (+ wins fails)))))

;; !I/O part

(defn print-statistics
  "Final result has to be like 'Faced up with 4 : 50%'"
  [& {:keys [id value] :or {id -1 value -1}}]
  (println (str "Faced up with " id " : " value)))

(defn give-statistics
  "Gather information from dealer-starts-with for i'th card"
  [i]
  (map-to-vec (assoc {} :id i :value (* 100 (dealer-starts-with i 0 0 1000)))))

;; !Helpers' part

(defn wide-into
  "Into macro for variety of args"
  ([a b]
   (into a b))
  ([a b & args]
   (into a (apply wide-into b args))))

(defn map-to-vec
  "Converts map to vector"
  [arg]
  (apply wide-into (map identity arg)))

;; !Main part

(defn main
  []
  (->>
   (range)
   (take 12)
   (drop 2)
   (map give-statistics)
   (map #(apply print-statistics %))))
(main)
