package ija.model.board;

import ija.model.cards.Card;
import ija.model.cards.CardDeck;
import ija.model.cards.CardStack;


public abstract class AbstractFactorySolitaire {

    public AbstractFactorySolitaire(){
    }

    public abstract Card createCard(Card.Color color, int value);

    public abstract CardDeck createCardDeck();

    public abstract CardDeck createTargetPack(Card.Color color);

    public abstract CardStack createWorkingPack();
}
