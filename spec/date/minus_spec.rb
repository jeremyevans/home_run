require File.expand_path('../../spec_helper', __FILE__)

describe "Date#-" do

  it "should substract a number of days from a Date" do
    (Date.civil(2008, 1, 8) - 315).should == Date.civil(2007,2,27)
    (Date.commercial(2007, 47, 2) - 315).should == Date.commercial(2007,2,2)
    (Date.jd(2455097) - 315).should == Date.jd(2454782)
    (Date.ordinal(2008, 325) - 315).should == Date.ordinal(2008, 10)
  end
  
  it "should substract a negative number of days from a Date" do
    d = Date.civil(2007, 4, 19).-(-13)
    d.should == Date.civil(2007, 5 ,2) 
  end

  it "should keep the same class as the receiver" do
    c = Class.new(Date)
    c.jd.-(10).should be_kind_of(c)
  end

  it "should be able to compute the different between two dates" do
    (Date.civil(2007,2,27) - Date.civil(2007,2,27)).should == 0
    (Date.civil(2007,2,27) - Date.civil(2007,2,26)).should == 1
    (Date.civil(2006,2,27) - Date.civil(2007,2,27)).should == -365
    (Date.civil(2008,2,27) - Date.civil(2007,2,27)).should == 365
    (Date.civil(2009,2,27) - Date.civil(2008,2,27)).should == 366
    
    (Date.civil(2009,2,27) - Date.commercial(2008,2,1)).should == 417
    (Date.civil(2009,2,27) - Date.jd(2454782)).should == 108
    (Date.civil(2009,2,27) - Date.ordinal(2008, 10)).should == 414
    
    (Date.commercial(2008,2,1) - Date.civil(2008,2,27)).should == -51
    (Date.commercial(2008,2,1) - Date.jd(2454782)).should == -309
    (Date.commercial(2008,2,1) - Date.ordinal(2008, 10)).should == -3
    
    (Date.jd(2454782) - Date.commercial(2008,2,1)).should == 309
    (Date.jd(2454782) - Date.civil(2009,2,27)).should == -108
    (Date.jd(2454782) - Date.ordinal(2008, 10)).should == 306
    
    (Date.ordinal(2008, 10) - Date.commercial(2008,2,1)).should == 3
    (Date.ordinal(2008, 10) - Date.jd(2454782)).should == -306
    (Date.ordinal(2008, 10) - Date.civil(2009,2,27)).should == -414
  end

  it "should be able to compute the difference between a Date and a DateTime" do
    (Date.civil(2007, 4, 19) - DateTime.civil(2007, 4, 18, 12)).should == 0.5
  end

  it "should raise an error on non numeric parameters" do
    lambda { Date.civil(2007,2,27) - :hello }.should raise_error(TypeError)
    lambda { Date.civil(2007,2,27) - "hello" }.should raise_error(TypeError)
    lambda { Date.civil(2007,2,27) - Object.new }.should raise_error(TypeError)
  end
  
end
