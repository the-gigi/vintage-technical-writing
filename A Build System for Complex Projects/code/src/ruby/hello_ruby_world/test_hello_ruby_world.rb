require 'hello_ruby_world' 
require 'test/unit'

class TestHelloWorld < Test::Unit::TestCase 
  def test_HelloWorld 
    hw = HelloWorld.new 
    assert_equal(hw.get_hello(), "hello")
    assert_equal(hw.get_world(), "world") 
  end 
end 
