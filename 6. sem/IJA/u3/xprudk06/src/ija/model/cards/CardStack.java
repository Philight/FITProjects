package ija.model.cards;

//package cards;

public class CardStack extends CardDeck
{
	public CardStack(int size)
	{
		super(size);
	}

	@Override
    public boolean put(Card c)
    {
        if (isEmpty()) 
        {
            if (c.value() != 13) 
                return false;  
        }
        else
        {
	     	if (get().value() - 1 != c.value()) 
        		return false;
        	if (get().similarColorTo(c)) 
        		return false;
        }
   
        cards[++top] = c;
        return true;
    }

	public boolean put(CardStack stack)
	{
		if (stack.isEmpty()) 
			return false;
		if (this.isEmpty()) 
		{
            if (stack.get().value() != 13) 
                return false;
        }

		CardStack tmpStack = new CardStack(52);	// temp stack for card order reverse
		while ( !stack.isEmpty() )
		{
			tmpStack.addTop(stack.cards[stack.top]);
			stack.top--;
		}

		while ( !tmpStack.isEmpty() )		// put in new Stack
		{
			cards[++top] = tmpStack.cards[tmpStack.top];
			tmpStack.top--;
		}

		return true;
	}

	public CardStack pop(Card card)
	{
		CardStack tmpStack = new CardStack(52);
		boolean found = true;

		do 
		{
			if (this.isEmpty() == true)
			{
				found = false;
				break;
			}
			tmpStack.addTop(cards[top]);
			top--; 		// of the taken stack
		} while (card.value() != cards[top+1].value()  || card.color() != cards[top+1].color());


		if (found)
		{
			CardStack reverseStack = new CardStack(52);
			while ( !tmpStack.isEmpty() )		
			{
				reverseStack.addTop(tmpStack.cards[tmpStack.top]); // put in new Stack
				tmpStack.top--;
			}
			return reverseStack;			
		}
		else
			return null;			

	}

	@Override
	public boolean equals(Object obj)
	{
        if (obj == null)
            return false;

        final CardStack stack = (CardStack) obj;
        if (this.top != stack.top)
        	return false;

        int origTop = this.top;
        while (origTop != -1)
        {
        	//System.out.println( "comparing "+this.cards[origTop]+" and "+stack.cards[origTop]);
        	if (!this.cards[origTop].equals(stack.cards[origTop]))
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

        CardStack stack1 = new CardStack(5);
        CardStack stack2 = new CardStack(5);
        CardStack stack3;
        
        stack1.put(new Card(Card.Color.CLUBS,2));
        stack1.put(new Card(Card.Color.SPADES,9));

        stack3 = stack1.pop(new Card(Card.Color.CLUBS,2));
        System.out.println( "Karta: "+stack3.cards[0].toString()+" || Stack3Size: "+stack3.size() );
        if (stack3 == null)
        	System.out.println( "empty" );

        stack2.put(new Card(Card.Color.DIAMONDS,1));
        stack2.put(new Card(Card.Color.HEARTS,2));
        stack2.put(new Card(Card.Color.SPADES,3));
        stack2.put(new Card(Card.Color.DIAMONDS,4));
        stack2.put(new Card(Card.Color.HEARTS,5));
        // stack1 = empty, put stack2
        stack1.put(stack2);
        System.out.println( "Karta: "+stack1.cards[0].toString()+" || Stack1Size: "+stack1.size() );
        System.out.println( "Karta: "+stack1.cards[1].toString());
        System.out.println( "Karta: "+stack1.cards[2].toString());
    	System.out.println( "Karta: "+stack1.cards[3].toString());
		System.out.println( "Karta: "+stack1.cards[4].toString());
	}*/
}