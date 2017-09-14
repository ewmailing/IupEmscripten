local lpeg = require("lpeg")
local re = require("re")



--[====[
record = re.compile[[
  record <- ( <field> (',' <field>)* ) -> {} ('%nl' / !.)
  field <- <escaped> / <nonescaped>
  nonescaped <- { [^,"%nl]* }
  escaped <- '"' {~ ([^"] / '""' -> '"')* ~} '"'
]]


result = lpeg.match (record, [[all,cows,"every good boy ""deserves"" fruit",eat,grass]])


if result then
	for k,v in pairs(result) do
		print(k, v)
	end
else
  print "no match"
end -- if

--[[
Output
1="all"
2="cows"
3="every good boy "deserves" fruit"
4="eat"
5="grass"
--]]

p = re.compile[[
      text <- {~ item* ~}
      item <- macro / [^()] / '(' item* ')'
      arg <- ' '* {~ (!',' item)* ~}
      args <- '(' arg (',' arg)* ')'
      -- now we define some macros
      macro <- ('apply' args) -> '%1(%2)'
             / ('add' args) -> '%1 + %2'
             / ('mul' args) -> '%1 * %2'
]]

print(p:match"foo add(mul(a,b), apply(f,x))")   --> a * b + f(x)


p = re.compile[[
      text <- {~ item* ~}
      item <- macro / [^()] / '(' item* ')'
      arg <- ' '* {~ (!',' item)* ~}
      args <- '(' arg (',' arg)* ')'
      -- now we define some macros
      macro <- ('apply' args) -> '%1(%2)'
             / ('add' args) -> '%1 + %2'
             / ('mul' args) -> '%1 * %2'
			 / ('foo') -> 'bar'
			 / ('feefoo') -> 'narf'
]]

print(p:match"foo feefoobaz add(mul(a,b), apply(f,x))")   --> a * b + f(x)
--]====]



--local welcomestring = "Welcome to the wonderful world of the Sweedish Chef! Enclosed in this archive are four files:"
local welcomestring = [[
Welcome to the wonderful world of the Sweedish Chef! Enclosed
in this archive are four files:

1) README, which you are reading
2) README.bork, which you might also be reading
3) chef.x, the lex source code to the Encheferizer

To compile, first run chef.x through lex with the command:
% lex chef.x

Now you have a file named lex.yy.c in the same directory.
Compile this C program with:
% cc -o chef lex.yy.c -ll

To use chef, redirect text into and out of it. For example:
% chef <README >README.bork

So, there you have it! Encheferizing has never been so easy. But
don't let this "modern miracle" make you lazy... try your hand at
manual Encheferizing ever once in a while. You'll find it's fun and
relaxing!

--
Jeff Allen <jallen@jarthur.claremont.edu>	Bork, bork, bork!
	   <jallen@fenris.claremont.edu>

April 30, 1993]]

--[[Velcume-a tu zee vunderffool vurld ooff zee Sveedeesh Cheff! Inclused
in thees ercheefe-a ere-a fuoor feeles:
--]]

bork = re.compile[[
	text <- {~ item* ~}
	WordChar <- [A-Za-z']
	NotWord <- [^A-Za-z']
	item <- ProcessedWord / NotWord

	ExemptWord <- 'bork'
		/ 'Bork'

	EndOfParagraphPunctuation <- [.!?]%nl -> '
Bork Bork Bork!
'
  	AccentSyllable <- 'an' -> 'un'
		/ 'An' -> 'Un'
		/ 'au' -> 'oo'
		/ 'Au' -> 'Oo'
		/ 'the' -> 'zee'
		/ 'The' -> 'Zee'
		/ 'v' -> 'f'
		/ 'V' -> 'F'
		/ 'w' -> 'v'
		/ 'W' -> 'V'

	AccentPrefixSubstitution <- 'e' -> 'i'
		/ 'E' -> 'I'
		/ 'o' -> 'oo'
		/ 'O' -> 'Oo'

	AccentSuffixSubstitution <- 'en' -> 'ee'
		/ 'th' -> 't'

	InWordAccentSuffixSubstitution  <-  'e' -> 'e-a'

	InWordAccentSubstituion <- 'ew' -> 'oo'
		/ 'f' -> 'ff'
		/ 'ir' -> 'ur'
		/ 'ow' -> 'oo'
		/ 'o' -> 'u'
		/ 'tion' -> 'shun'
		/ 'u' -> 'oo'
		/ 'U' -> 'Oo'

		/ 'i' -> '||ee||'
		
	
	AccentFollowedBySyllableSubstitution <-  'an' -> 'un'
		/ 'An' -> 'Un'
		/ 'au' -> 'oo'
		/ 'Au' -> 'Oo'

		/ 'a' -> 'e'
		/ 'A' -> 'E'


	CombinedInWordAccent <- InWordAccentSubstituion / AccentSyllable
	CombinedInWordAccentThatHasNoSuffixOrChar <- CombinedInWordAccent / WordChar
	CombinedInWordAccentThatHasSuffix <- AccentFollowedBySyllableSubstitution / CombinedInWordAccent	
	CombinedInWordAccentThatHasSuffixOrChar <- CombinedInWordAccentThatHasSuffix / WordChar

	CombinedAnyTimeSuffix <-  AccentSuffixSubstitution / InWordAccentSuffixSubstitution

	CombinedAccentSyllableOrChar <- AccentSyllable / WordChar
	CombinedAccentSyllableOrCharThatWillBeFollowedByAnotherCharacter <- AccentFollowedBySyllableSubstitution / CombinedAccentSyllableOrChar




	ProcessedInWordAccentThatEndsWithSuffix <- CombinedAnyTimeSuffix NotWord ProcessedWord
		/ CombinedAnyTimeSuffix NotWord 
		/ CombinedInWordAccentThatHasSuffix
		/ WordChar
	
	ProcessedInWordAccentThatEndsWithNoSuffix <- CombinedInWordAccentThatHasSuffixOrChar CombinedInWordAccent NotWord item
		/ CombinedInWordAccentThatHasSuffixOrChar CombinedInWordAccent NotWord
		/ CombinedInWordAccentThatHasSuffix
		/ WordChar	
	
	ProcessedWord <- ExemptWord 
		/ EndOfParagraphPunctuation
		/ AccentPrefixSubstitution ProcessedInWordAccentThatEndsWithSuffix+
		/ AccentPrefixSubstitution NotWord item
		/ CombinedAccentSyllableOrCharThatWillBeFollowedByAnotherCharacter ProcessedInWordAccentThatEndsWithSuffix+
		/ CombinedAccentSyllableOrCharThatWillBeFollowedByAnotherCharacter ProcessedInWordAccentThatEndsWithNoSuffix+
		/ CombinedAccentSyllableOrChar NotWord item
		/ AccentSuffixSubstitution NotWord item

]]

--[[
--	ProcessedWord <- ExemptWord NotWord
		/ EndOfParagraphPunctuation
		/ AccentPrefixSubstitution ProcessedInWordAccentThatEndsWithSuffix+ 
		/ CombinedAccentSyllableOrCharThatWillBeFollowedByAnotherCharacter ProcessedInWordAccentThatEndsWithSuffix+
		/ CombinedAccentSyllableOrCharThatWillBeFollowedByAnotherCharacter ProcessedInWordAccentThatEndsWithNoSuffix+
		/ CombinedAccentSyllableOrChar NotWord
		/ AccentSuffixSubstitution NotWord

--]]

-- CombinedInWordAccent: Inside a word, evaluates substitions in a word (InWordAccentSubstituion) or any general substitution (AccentSyllable).
-- CombinedInWordAccentThatHasNoSuffixOrChar: Inside a word. Like CombinedInWordAccent except it also matches a WordChar as a last match. Intended to be used for matches inside a word, but those words won't have a well known suffix.
-- CombinedInWordAccentThatHasSuffix: Inside a word and assumes will be ended with a suffix. Evaluates CombinedInWordAccent plus AccentFollowedBySyllableSubstitution (because I know there will be following characters due to the suffix). 
-- CombinedInWordAccentThatHasSuffixOrChar: Inside a word and assumes will be ended with a suffix. Evaluates CombinedInWordAccentThatHasSuffix plus matches WordChar as the last match.
-- CombinedAnyTimeSuffix: Any suffix (two types of suffixes: those that are in a word, and those that are the entire word by themselves).
-- CombinedAccentSyllableOrChar: Evalutes general substitutions that may occur anywhere (AccentSyllable) or matches the WordChar.
-- CombinedAccentSyllableOrCharThatWillBeFollowedByAnotherCharacter: Assumes not at the end of a word. Like CombinedAccentSyllableOrChar but also evaluates AccentFollowedBySyllableSubstitution because there are following characters which allow this pattern to be matched.

-- ProcessedInWordAccentThatEndsWithSuffix: Intended to get all patterns inside a word that end with a suffix. (Use of repetitions expected.) 
--
-- ProcessedInWordAccentThatEndsWithNoSuffix: Intended to get all patterns inside a word that don't end with a suffix. (Use of repetitions expected.) The pattern terminology used inside here is a little screwed up. Even though internally it uses 'HasSuffix', its intent is merely to make sure AccentFollowedBySyllableSubstitution patterns are evaluated. This is done by explicitly having 'CombinedInWordAccent NotWord' at the end of the first pattern which ensures there will be following characters.

-- ProcessedWord:
-- 1) exempt full words 'bork' and 'Bork' from being transformed.
-- 2) Transform all end of paragraphs with .?! to
-- \nBork Bork Bork!\n
-- 3) Handle words with both a prefix and suffix
-- 4) Handle words that are just a prefix by itself
-- 5) Handle words with no prefix and suffix
-- 6) Handle words with no prefix and no suffix
-- 7) Handle words that are just a special syllable by itself
-- 8) Handle works that are just a suffix by itself


-- Special notes:
-- AccentFollowedBySyllableSubstitution: It partly repeats AccentSyllable at the beginning because those characters take precedent in the origianl lex.



--[[
AccentSubstituion fails if it doesn't match one of the patterns.
This causes AccentPrefix to fail and thus Word goes on to the alternative pattern.
--]]

--[[
--
		/ CombinedAccentSyllableOrChar CombinedAccentSyllableOrChar UGHCOMBINED3+ 'com' CombinedAnyTimeSuffix
--
	ProcessedWord <- ExemptWord 
		/ AccentPrefixSubstitution CombinedInWordAccentThatHasSuffixOrChar+ CombinedAnyTimeSuffix
		/ AccentPrefixSubstitution CombinedAnyTimeSuffix
		/ CombinedAccentSyllableOrChar CombinedInWordAccentThatHasSuffixOrChar+ CombinedAnyTimeSuffix
		/ CombinedAccentSyllableOrChar CombinedInWordAccentThatHasSuffixOrChar+ CombinedInWordAccent
		/ CombinedAccentSyllableOrChar CombinedInWordAccentThatHasNoSuffixOrChar
		/ AccentSuffixSubstitution
		/ WordChar+


	ProcessedWord <- ExemptWord 
		/ CombinedAccentSyllableOrChar CombinedAccentSyllableOrChar UGHCOMBINED3+
		/ CombinedAccentSyllableOrChar CombinedInWordAccentThatHasSuffixOrChar+ CombinedAnyTimeSuffix
		/ WordChar+


--]]
--[[
	Syllable <- AccentSyllable / AccentSyllable AccentSuffix %W / %w AccentSuffix %W/ %w

	Syllable <- AccentSyllable / AccentSyllable AccentSuffix %W / %w AccentSuffix %W/ %w
--
	text <- {~ item* ~}


		/ %w+		
--]]
--[[
	text <- {~ item* ~}
	item <- Word / NotWord
	the <- 'the'-> 'zee'
	NotWord <- !<Word>
  	Word <- [A-Za-z']
--]]
result = lpeg.match (bork, welcomestring)

--[[
if result then
	if type(result) == "table" then
		print("table")
		for k,v in pairs(result) do
			print(k, v)
		end
	else
		print("not table")
		print(result)
	end
else
  print "no match"
end -- if
--]]


postee = re.compile[[
	text <- {~ item* ~}
	WordChar <- [A-Za-z']
	NotWord <- [^A-Za-z']
	item <- ProcessedWord / NotWord

	Transform1 <- '||ee||' -> 'ee'
	Transform2 <- '||ee||' -> 'i'


	ProcessedWord <- WordChar+ Transform1 WordChar+ Transform2*
	/ WordChar+ Transform1 Transform2*
	/ WordChar+ Transform1 
	/ WordChar
]]

result = lpeg.match (postee, result)

if result then
	if type(result) == "table" then
--		print("table")
		
		for k,v in pairs(result) do
			print(k, v)
		end
	else
--		print("not table")
		
		print(result)
	end
else
  print "no match"
end -- if


--[[
Velcume-a tu zee vunderffool vurld ooff zee Sveedeesh Cheff! Inclused
in thees ercheefe-a ere-a fuoor feeles:

1) REEDME, vheech yuoo ere-a reedeeng
2) REEDME.bork, vheech yuoo meeght elsu be-a reedeeng
3) cheff.x, zee lex suoorce-a cude-a tu zee Incheffereezer
4) cheff.c, zee C cude-a prudooced by my ferseeun ooff lex.
Bork Bork Bork!

Tu cumpeele-a, furst roon cheff.x thruoogh lex veet zee cummund:
% lex cheff.x

Noo yuoo hefe-a a feele-a nemed lex.yy.c in zee seme-a durectury.
Bork Bork Bork!
Cumpeele-a thees C prugrem veet:
% cc -oo cheff lex.yy.c -ll

Tu use-a cheff, redurect text intu und oooot ooff it. Fur ixemple-a:
% cheff <REEDME >REEDME.bork

Iff fur sume-a reesun yuoor cumpooter duesn't hefe-a lex, yuoo mey be-a
eble-a tu cumpeele-a cheff.c, thuoogh it reqooures zee lex leebreries.
Bork Bork Bork!
Unyvey, zee cummund tu du thees is:
%cc -oo cheff cheff.c -ll

Su, zeere-a yuoo hefe-a it! Incheffereezing hes nefer beee su iesy. Boot
dun't let thees "mudern murecle-a" meke-a yuoo lezy... try yuoor hund et
munooel Incheffereezing ifer oonce-a in a vheele-a. Yuoo'll feend it's foon und
relexeeng!

Jeffff Ellee <jellee@jerthoor.cleremunt.idoo>	Bork, bork, bork!
	   <jellee@fenrees.cleremunt.idoo>

December 16, 1992
--]]

fudd = re.compile[[
	text <- {~ item* ~}
	WordChar <- [A-Za-z']
	NotWord <- [^A-Za-z']
	item <- ProcessedWord


	EndOfParagraphPunctuation <- [.!?]%nl -> '
uh-uh-uh-uhuh-uh-uh-uh.
'
  	AccentSyllable <- 'r' -> 'w'
		/ 'l' -> 'w'
		/ 'qu' -> 'qw'
		/ 'th ' -> 'f '
		/ 'th' -> 'd'
		/ 'R' -> 'W'
		/ 'Qu' -> 'Qw'
		/ 'QU' -> 'QW'
		/ 'TH ' -> 'F '
		/ 'TH' -> 'D'
		/ 'Th' -> 'D'
	
	ProcessedWord <- AccentSyllable
		/ EndOfParagraphPunctuation
		/ .

]]

function Translate(the_string)
	local first_passresult = lpeg.match(bork, tostring(the_string))
	local final_result = lpeg.match(postee, first_passresult)
	return final_result
end


function TranslateF(the_string)
	local final_result = lpeg.match(fudd, tostring(the_string))
	return final_result
end






borksynth = re.compile[[
	text <- {~ item* ~}
	WordChar <- [A-Za-z']
	NotWord <- [^A-Za-z']
	item <- ProcessedWord / NotWord

	ExemptWord <- 'bork'
		/ 'Bork'

	EndOfParagraphPunctuation <- [.!?]%nl -> '
Bork Bork Bork!
'
  	AccentSyllable <- 'an' -> 'un'
		/ 'An' -> 'Un'
		/ 'au' -> 'oo'
		/ 'Au' -> 'Oo'
		/ 'the' -> 'zee'
		/ 'The' -> 'Zee'
		/ 'v' -> 'f'
		/ 'V' -> 'F'
		/ 'w' -> 'v'
		/ 'W' -> 'V'

	AccentPrefixSubstitution <- 'e' -> 'i'
		/ 'E' -> 'I'
		/ 'o' -> 'oo'
		/ 'O' -> 'Oo'

	AccentSuffixSubstitution <- 'en' -> 'ee'
		/ 'th' -> 't'

	InWordAccentSuffixSubstitution  <-  'e' -> 'ea'

	InWordAccentSubstituion <- 'ew' -> 'oo'
		/ 'f' -> 'f'
		/ 'ir' -> 'ur'
		/ 'ow' -> 'oo'
		/ 'o' -> 'u'
		/ 'tion' -> 'shun'
		/ 'u' -> 'oo'
		/ 'U' -> 'Oo'

		/ 'i' -> '||ee||'
		
	
	AccentFollowedBySyllableSubstitution <-  'an' -> 'un'
		/ 'An' -> 'Un'
		/ 'au' -> 'oo'
		/ 'Au' -> 'Oo'

		/ 'ar' -> 'ar'
		/ 'Ar' -> 'Ar'
		/ 'a' -> 'e'
		/ 'A' -> 'E'


	CombinedInWordAccent <- InWordAccentSubstituion / AccentSyllable
	CombinedInWordAccentThatHasNoSuffixOrChar <- CombinedInWordAccent / WordChar
	CombinedInWordAccentThatHasSuffix <- AccentFollowedBySyllableSubstitution / CombinedInWordAccent	
	CombinedInWordAccentThatHasSuffixOrChar <- CombinedInWordAccentThatHasSuffix / WordChar

	CombinedAnyTimeSuffix <-  AccentSuffixSubstitution / InWordAccentSuffixSubstitution

	CombinedAccentSyllableOrChar <- AccentSyllable / WordChar
	CombinedAccentSyllableOrCharThatWillBeFollowedByAnotherCharacter <- AccentFollowedBySyllableSubstitution / CombinedAccentSyllableOrChar




	ProcessedInWordAccentThatEndsWithSuffix <- CombinedAnyTimeSuffix NotWord ProcessedWord
		/ CombinedAnyTimeSuffix NotWord 
		/ CombinedInWordAccentThatHasSuffix
		/ WordChar
	
	ProcessedInWordAccentThatEndsWithNoSuffix <- CombinedInWordAccentThatHasSuffixOrChar CombinedInWordAccent NotWord item
		/ CombinedInWordAccentThatHasSuffixOrChar CombinedInWordAccent NotWord
		/ CombinedInWordAccentThatHasSuffix
		/ WordChar	
	
	ProcessedWord <- ExemptWord 
		/ EndOfParagraphPunctuation
		/ AccentPrefixSubstitution ProcessedInWordAccentThatEndsWithSuffix+
		/ AccentPrefixSubstitution NotWord item
		/ CombinedAccentSyllableOrCharThatWillBeFollowedByAnotherCharacter ProcessedInWordAccentThatEndsWithSuffix+
		/ CombinedAccentSyllableOrCharThatWillBeFollowedByAnotherCharacter ProcessedInWordAccentThatEndsWithNoSuffix+
		/ CombinedAccentSyllableOrChar NotWord item
		/ AccentSuffixSubstitution NotWord item

]]


function TranslateSynth(the_string)
	local first_passresult = lpeg.match(borksynth, tostring(the_string))
	local final_result = lpeg.match(postee, first_passresult)
	return final_result
end


function TranslateSynthF(the_string)
	local final_result = lpeg.match(fudd, tostring(the_string))
	return final_result
end



