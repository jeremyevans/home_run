require File.expand_path('../../spec_helper', __FILE__)

describe "Date#gregorian?" do
  it "should return true" do
    Date.civil(1007, 2, 27).gregorian?.should == true
    Date.civil(1907, 2, 27, Date.civil(2000, 1, 1).jd).gregorian?.should == true
  end
  
  ruby_version_is "" ... "1.9" do
    it "ns? should be same as gregorian?" do
      Date.civil(1007, 2, 27).ns?.should == true
    end
  end
end

describe "Date.gregorian_leap?" do

  it "should be able to determine whether a year is a leap year in the Gregorian calendar" do
    Date.gregorian_leap?(1900).should == false
    Date.gregorian_leap?(1999).should == false
    Date.gregorian_leap?(2000).should == true
    Date.gregorian_leap?(2002).should == false
    Date.gregorian_leap?(2004).should == true
    Date.gregorian_leap?(1901).should == false
    Date.gregorian_leap?(1899).should == false
    Date.gregorian_leap?(1904).should == true
    Date.gregorian_leap?(1896).should == true
    Date.gregorian_leap?(1999).should == false
    Date.gregorian_leap?(2001).should == false
  end

  it ".leap? should be the same as gregorian_leap" do
    Date.leap?(1900).should == Date.gregorian_leap?(1900)
    Date.leap?(2000).should == Date.gregorian_leap?(2000)
  end
end

ruby_version_is "" ... "1.9" do
  describe "Date" do
    it ".gregorian? should return whether the first argument is greater than or equal the second" do
      Date.gregorian?(1, 2).should == false
      Date.gregorian?(2, 1).should == true
      Date.gregorian?(1, 1).should == true
    end

    it ".ns? should be the same as gregorian?" do
      Date.ns?(2, 1).should == Date.gregorian?(2, 1)
      Date.ns?(1, 2).should == Date.gregorian?(1, 2)
      Date.ns?(1, 1).should == Date.gregorian?(1, 1)
    end
  end
end
