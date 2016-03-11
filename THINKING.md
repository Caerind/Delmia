# Delmia - Thinking Document
============================

List Actors and their relations/links and the actions relied to them.

The architecture we are thinking about look like this :

			Entity
			   |
			   |
	   |----------------|
	   |                |
	   V                V
	  Unit           Building
	  
	  
What do they represent ?

- Entity represent the common base. 
Which is actuallty Life and ResourceHolder properties. Entity is obliged because of Warriors which can target even building and unit.
- Unit represent an unit on the world (mean that not follow tiles). 
An unit as movement and smart animations and also the AI to control it.
- Building represent a building or a resource on the map (mean that follow tiles). 
A Bulding as animations connected on its life state and also properties to follow tiles.


Now the base architecture is good, we need to think about the Unit and Building we will create.

List of Units, and their specifications :
- Citizen += ResourceGatherer, Builder, Fighter
- Warrior += Fighter
- Merchant += Trader

List of Building, and their specifications :
- House += Housing
- Hall += Hall
- Forest += Gatherable // Note : Maybe Gatherable should inherit from Entity instead ? Or Gatherable as a property in 

(This is just a short list at the moment)

