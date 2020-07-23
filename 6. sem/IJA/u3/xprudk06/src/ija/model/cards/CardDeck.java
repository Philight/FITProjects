package ija.model.cards;
import ija.model.cards.Card;

/*package cards;
import cards.Card;*/

import java.util.HashSet;
import java.util.Set;

public class CardDeck
{
	protected int deckSize;
	protected int top;
	protected Card[] cards;
	protected Card.Color deckCol;

	public CardDeck(int size)
	{
		deckSize = size;
		top = -1;
		cards = new Card[deckSize];
	}

	public int size()
	{
		return (this.top + 1);
	}

	public void setColor(Card.Color c)
	{
		deckCol = c;
	}
	
	public void addTop(Card card)
	{
		cards[++top] = card;
	}

	public boolean put(Card card)
	{
		if (card==null) 
			return false;
		if (this.size() == deckSize)
			return false;

		if (deckCol != null)
		{
			if (!deckCol.equals(card.color()))
				return false;
			if (this.size() + 1 != card.value()) 
				return false;
		}
		
		cards[++top] = card;
		// TODO
		return true;
	}

	public Card pop()
	{
		if (top == -1) 
			return null;
		Card returnCard = cards[top];
		top--;
		return returnCard;
	}

	public Card get()
	{
		if (this.isEmpty())
			return null;
		else
			return cards[top];
	}

	public Card get(int index)
	{
		if (this.isEmpty() || (index>top || index<0))
			return null;
		return cards[index];
	}

	public boolean isEmpty()
	{
		if (top==-1)
			return true;
		else
			return false;
	}
	@Override
	public boolean equals(Object obj)
	{
        if (obj == null)
            return false;

        final CardDeck deck = (CardDeck) obj;
        if (this.top != deck.top)
        	return false;

        int origTop = this.top;
        while (origTop != -1)
        {
        	//System.out.println( "comparing "+this.cards[origTop]+" and "+stack.cards[origTop]);
        	if (!this.cards[origTop].equals(deck.cards[origTop]))
        		return false;
        	origTop--;
        }
   		return true;
	}
/*
	public static void main(String[] args)
	{
		boolean assertion = false;
        assert assertion = true;
        if (! assertion)
        {
            System.out.println("Spustte s volbou -ea");
            return;
        }

        CardDeck deck = new CardDeck(52);
        assert deck.size() == 52 : "Pocet karet je 52";
        System.out.println("Pocet karet je 52 OK");


        Card card1 = new Card(Card.Color.CLUBS,1);
        Card card2 = new Card(Card.Color.CLUBS,1);
        if (card1.equals(card2))
  	    	System.out.println("Karty su OK");
  	    System.out.println(""+Card.Color.valueOf("CLUBS"));
  	    
	  	
      




        Set<Card> testCards = new HashSet<>();
        for (int i = 1; i <= 13; i++) { testCards.add(new Card(Card.Color.CLUBS,i)); }
        for (int i = 1; i <= 13; i++) { testCards.add(new Card(Card.Color.DIAMONDS,i)); }
        for (int i = 1; i <= 13; i++) { testCards.add(new Card(Card.Color.HEARTS,i)); }
        for (int i = 1; i <= 13; i++) { testCards.add(new Card(Card.Color.SPADES,i)); }
        
        for (int i = 0; i < 52; i++) {
            Card c = deck.pop();
            System.out.println("test karty["+i+"] = "+c.toString());
            assert testCards.remove(c) : "Spravna karta";
        }
        
        assert testCards.isEmpty() : "Testova sada musi byt prazdna";  
	}*/
}