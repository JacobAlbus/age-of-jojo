# Age of Jojo

Based off of the hit flash game *Age of War* and with inspiration from Jojo's Bizzare Adven ture, send your favorite Jojo characters to battle and defeat the dastardly Dio! However, turrets were taken out because they break the game and special events were removed to make the game balanced for each side. The latter point is of importance since the original goal of this project was to create an AI enemy that can beat the human player (or at the very least bring to a stalemate since the "optimal" strategy is always to just copy the opponent's moves). In order to create a strong AI enemy that can beat the human player, it thus became necessary to make both sides equal in power and moves. 

--- 

You too can create your own enemy AI by either modifiying the `AIController` class or creating your own derived class from the `BaseController` class. If you create your own derived class, just add it to the `BattleEngine` class as necessary
