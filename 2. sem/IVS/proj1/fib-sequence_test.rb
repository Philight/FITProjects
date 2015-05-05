require 'test/unit'
require_relative 'fib-sequence'

# Fib: 0 1 1 2 3 5 8 13  21  34  55  89  144 ...
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# testy current a current_idx su zahrnute v ostatnych testoch #
class FibonacciSequenceTest < Test::Unit::TestCase
  def setup
  @sequence = FibonacciSequence.new
  end

  # test inicializacie triedy, po vytvoreni sa ma aktualny clen(current) a index akt. clenu(current_idx) rovnat [nil]
  def test_initialize
  	@sequence.instance_eval{ initialize }
  	assert_equal( nil, @sequence.current )
  	assert_equal( nil, @sequence.current_idx )
  end

  # nastavi radu na dany index a vyhodnoti clena
  def test_generator
    assert_equal( 8, @sequence[6] )
    assert_equal( 6, @sequence.current_idx )

    assert( @sequence[-2].nil? ) # test neplatny index
  end
 
  def test_reset
    @sequence[10]
    @sequence.reset
    assert_equal( nil, @sequence.current )
    assert_equal( nil, @sequence.current_idx )
  end

  def test_next
    @sequence[0]
    for i in 1..7 do # 7. clen postupnosti
      @sequence.next
    end
    
    assert_equal( 13, @sequence.current )
  end
 
end