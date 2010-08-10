require File.expand_path('../../spec_helper', __FILE__)

describe "Date#ordinal" do

  it "should be able to construct a Date object from an ordinal date" do
    lambda { Date.ordinal(1900, 366) }.should raise_error(ArgumentError)
    lambda { Date.ordinal(1900, 365) }.should_not raise_error(ArgumentError)
    lambda { Date.ordinal(2000, 366) }.should_not raise_error(ArgumentError)
    lambda { Date.ordinal(2000, 367) }.should raise_error(ArgumentError)
    Date.ordinal(2000, 288).should == Date.civil(2000, 10, 14)
  end

end
