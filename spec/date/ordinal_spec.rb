require File.expand_path('../../spec_helper', __FILE__)

describe "Date.ordinal" do

  it "should be able to construct a Date object from an ordinal date" do
    Date.ordinal(1582, 274).should == Date.civil(1582, 10,  1)
    Date.ordinal(1582, 277).should == Date.civil(1582, 10,  4)
    Date.ordinal(1582, 288).should == Date.civil(1582, 10, 15)
    Date.ordinal(1582, 287, Date::ENGLAND).should == Date.civil(1582, 10, 14, Date::ENGLAND)
  end
    
  it "should have defaults and an optional sg value" do
    Date.ordinal.should == Date.jd
    Date.ordinal(2008).should == Date.ordinal(2008, 1)
    Date.ordinal(2008, 1, 1).should == Date.ordinal(2008, 1)
  end
  
  it "should not accept more than 3 arguments" do
    proc{Date.ordinal(2008, 1, 1, 1)}.should raise_error(ArgumentError)
  end

  it "raises errors for invalid dates" do
    lambda { Date.ordinal(2007, 366) }.should raise_error(ArgumentError)
    lambda { Date.ordinal(2008, 366) }.should_not raise_error(ArgumentError)
    lambda { Date.ordinal(2008, 367) }.should raise_error(ArgumentError)
  end

  it "should keep the same class as the receiver" do
    c = Class.new(Date)
    c.ordinal.should be_kind_of(c)
  end

  ruby_version_is "" ... "1.9" do
    it ".new2 should be the same as ordinal" do
      Date.new2(2008, 10).should == Date.ordinal(2008, 10)
    end
  end
end

describe "Date.valid_ordinal?" do

  ruby_version_is "" ... "1.9" do
    it "should be able to determine if the date is a valid ordinal date" do
      Date.valid_ordinal?(1582, 277).should == Date.civil(1582, 10,  4).jd
      Date.valid_ordinal?(1582, 278).should == Date.civil(1582, 10,  5).jd
      Date.valid_ordinal?(1582, 287).should == Date.civil(1582, 10, 14).jd
      Date.valid_ordinal?(1582, 288).should == Date.civil(1582, 10, 15).jd
      Date.valid_ordinal?(1582, 287, Date::ENGLAND).should_not == nil
      Date.valid_ordinal?(1582, 287, Date::ENGLAND).should == Date.civil(1582, 10, 14, Date::ENGLAND).jd
      
      Date.valid_ordinal?(2007, 55).should == 2454156
      Date.valid_ordinal?(2007, 55, 1).should == 2454156
      Date.valid_ordinal?(2007, 367, 1).should == nil
    end
    
    it "#exist2? should be the same as valid_ordinal?" do
      Date.exist2?(2007, 55, 1).should == Date.valid_ordinal?(2007, 55)
    end

    it "should be able to handle negative day numbers" do
      Date.valid_ordinal?(1582, -89).should == Date.civil(1582, 10,  4).jd
      Date.valid_ordinal?(1582, -88).should == Date.civil(1582, 10,  5).jd
      Date.valid_ordinal?(1582, -79).should == Date.civil(1582, 10, 14).jd
      Date.valid_ordinal?(1582, -78).should == Date.civil(1582, 10, 15).jd
      Date.valid_ordinal?(2007, -100).should == Date.valid_ordinal?(2007, 266)
    end
  end

  ruby_version_is "1.9" do
    it "should be able to determine if the date is a valid ordinal date" do
      Date.valid_ordinal?(1582, 277).should == true
      Date.valid_ordinal?(1582, 278).should == true
      Date.valid_ordinal?(1582, 287).should == true
      Date.valid_ordinal?(1582, 288).should == true

      Date.valid_ordinal?(2007, 55).should == true
      Date.valid_ordinal?(2007, 55, 1).should == true
      Date.valid_ordinal?(2007, 367, 1).should == false
    end

    it "should be able to handle negative day numbers" do
      Date.valid_ordinal?(1582, -79).should == true
      Date.valid_ordinal?(1582, -78).should == true
      Date.valid_ordinal?(2007, -100).should == true
    end
  end

end
