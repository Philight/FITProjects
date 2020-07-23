
package ija.model.cards;

//package cards;

public class Card
{
	private Card.Color cardCol;
	private int cardVal;
	private boolean cardFaceUp;

	public static enum Color
	{ 
		CLUBS("C"), DIAMONDS("D"), HEARTS("H"), SPADES("S"); 

		private final String colorName;       
    	private Color(String s)
    	{
        	colorName = s;
    	}

		public String toString()
		{
			return this.colorName;
		}

		public boolean similarColorTo(Card.Color c)
		{
			if (colorName.equals(Card.Color.CLUBS.colorName) || colorName.equals(Card.Color.SPADES.colorName)) 
			{
				if (c.colorName.equals(Card.Color.CLUBS.colorName) || c.colorName.equals(Card.Color.SPADES.colorName))
					return true;
				
			}
			else if (colorName.equals(Card.Color.HEARTS.colorName) || colorName.equals(Card.Color.DIAMONDS.colorName)) 
			{
				if (c.colorName.equals(Card.Color.HEARTS.colorName) || c.colorName.equals(Card.Color.DIAMONDS.colorName))
					return true;
				
			}
			return false;
			
		}
	}

	public Card(Card.Color c, int value)
	{
		this.cardCol = c;
		this.cardVal = value;
		this.cardFaceUp = false;
	}

	public int value()
	{
		return this.cardVal;
	}

	public Card.Color color()
	{
		return this.cardCol;
	}

	public String toString()
	{
		String valueStr;
		switch(this.cardVal)
		{
			case 1 :
				valueStr = "A";
				break;
			case 11 :
				valueStr = "J";
				break;
			case 12 :
				valueStr = "Q";
				break;
			case 13 :
				valueStr = "K";
				break;
			default :
				valueStr = String.valueOf(this.cardVal);
		}

		return ( valueStr + "(" + (this.cardCol) + ")" );
	}
	
	@Override
	public boolean equals(Object obj)
	{
        if (obj == null)
            return false;
        final Card c = (Card) obj;
   		return (this.cardCol==c.cardCol) && (this.cardVal==c.cardVal);
	}

	@Override
	public int hashCode()
	{
		int hashC = 1;
		hashC = 7 * hashC + this.cardVal;
		hashC = 7 * hashC + this.cardCol.hashCode();
		return hashC;
	}

	public boolean isTurnedFaceUp()
	{
		return this.cardFaceUp;
	}

	public boolean turnFaceUp()
	{
		if (this.cardFaceUp)
			return false;
		else
		{
			cardFaceUp = true;
			return true;
		}
	}

	public boolean similarColorTo(Card c)
	{
		if (cardCol.equals(Card.Color.CLUBS) || cardCol.equals(Card.Color.SPADES)) 
			{
				if (c.cardCol.equals(Card.Color.CLUBS) || c.cardCol.equals(Card.Color.SPADES))
					return true;
				
			}
			else if (cardCol.equals(Card.Color.HEARTS) || cardCol.equals(Card.Color.DIAMONDS)) 
			{
				if (c.cardCol.equals(Card.Color.HEARTS) || c.cardCol.equals(Card.Color.DIAMONDS))
					return true;
				
			}
		return false;

	}

	public int compareValue(Card c)
	{
		return (this.cardVal-c.cardVal);
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

		Card c1 = new Card(Card.Color.CLUBS, 9);
        Card c2 = new Card(Card.Color.HEARTS, 11);
        Card c3 = new Card(Card.Color.CLUBS, 9);
		System.out.println(c1.cardCol+", "+c1.cardVal);
		System.out.println(c2.cardCol+", "+c2.cardVal);
		System.out.println(c3.cardCol+", "+c3.cardVal);

		assert c1.color().equals(Card.Color.CLUBS) : "Test barvy karty";
		System.out.println("Test barvy karty OK");

		assert c2.value() == 11 : "Test hodnoty karty";
		System.out.println("Test hodnoty karty OK");

		assert Card.Color.HEARTS.toString().equals("H") : "Test textove reprezentace barvy";
		System.out.println("Test textove reprezentace barvy OK");

		assert c1.toString().equals("9(C)") : "Retezec reprezentujici kartu 9(C)";
		System.out.println("Retezec reprezentujici kartu 9(C) OK");

		Card.Color c = Card.Color.valueOf("HEARTS");
    	System.out.println(c);
	
		System.out.println(c1.similarColorTo(c3));
		System.out.println(c1.compareValue(c2));
	}*/
}