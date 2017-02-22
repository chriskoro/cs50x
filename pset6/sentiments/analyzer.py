import nltk

class Analyzer():
    """Implements sentiment analysis."""
    

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        #Load pos and neg words into memory / structure so analyze can access them
        file = open(positives, "r")
        file2 = open(negatives, "r")
        
        self.positiveSet = set()
        self.negativeSet = set()
    
        for line in file:
            if line.startswith(";") == False:
                self.positiveSet.add(line.strip())

        for line in file2:
            if line.startswith(";") == False:
                self.negativeSet.add(line.strip())
        

    def analyze(self, text):
        """Analyze text (user entered argv[1]) for sentiment versus sets, returning its score."""
        #assign each word in text a value (-1,0,1); calculate text's total score
        
        #split up given text (each token is word) #tokenizer is a list of strings.  #each token contains single word
        #work with this tokens list like an array in C
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        
        n = len(tokens)
        score = 0

        for i in range(n):
            if tokens[i].lower() in self.positiveSet:
                score = score + 1
            elif tokens[i].lower() in self.negativeSet:
                score = score - 1
                
        print("Happiness rating: {}".format(score))
                    
        return score
