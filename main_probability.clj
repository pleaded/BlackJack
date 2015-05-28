(ns BlackJack)

(def row '(2 3 4 5 6 7 8 9 10 10 10 10 11))
(def deck (list row row row row))

(defn rcard []
  "Get random card from a deck"
  (rand-nth (rand-nth deck)))

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

;; C-like for macro
(defmacro for-loop [[sym init check change :as params] & steps]
 `(loop [~sym ~init value# nil]
    (if ~check
      (let [new-value# (do ~@steps)]
        (recur ~change new-value#))
      value#)))

(for-loop [i 2 (<= i 12) (inc i)]
    (println
     (str "Faced up card is ", i, " : ",
          (format "so %.1f%%" (* (dealer-starts-with i 0 0 1000) 100)))))