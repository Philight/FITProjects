package ija.model.board;

import ija.model.cards.Card;
import ija.model.cards.CardDeck;
import ija.model.cards.CardStack;

public class FactoryKlondike extends AbstractFactorySolitaire
{
    @Override
    public CardDeck createCardDeck()
    {
        CardDeck cardDeck = new CardDeck(52);

		for (Card.Color cardColor : Card.Color.values())
		{
			for (int i = 1; i<= 13; i++) {
					cardDeck.put(new Card(cardColor,i));
				}	
		}

        return cardDeck;
    }

	@Override
    public Card createCard(Card.Color color, int value)
    {
        if (value < 1 || value > 13)
            return null;
        return new Card(color, value);
    }


    @Override
    public CardDeck createTargetPack(Card.Color color)
    {
        CardDeck cardDeck = new CardDeck(52);
        cardDeck.setColor(color);
        return cardDeck;
    }

    @Override
    public CardStack createWorkingPack()
    {
        return new CardStack(52);
    }
}