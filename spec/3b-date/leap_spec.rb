require File.expand_path('../../spec_helper', __FILE__)

describe "Date#leap?" do
  it "should be true if the current date is a leap year and no argument is given" do
    Date.civil(2000, 10, 11).leap?.should ==  true
    Date.civil(2004, 10, 11).leap?.should ==  true
    Date.civil(2008, 10, 11).leap?.should ==  true
    Date.civil(1996, 10, 11).leap?.should ==  true
    Date.civil(1600, 10, 11).leap?.should ==  true
  end
  
  it "should be false if the current date is not a leap year and no argument is given" do
    Date.civil(1700, 10, 11).leap?.should ==  false
    Date.civil(1800, 10, 11).leap?.should ==  false
    Date.civil(1900, 10, 11).leap?.should ==  false
    Date.civil(1999, 10, 11).leap?.should ==  false
    Date.civil(2001, 10, 11).leap?.should ==  false
  end
end
