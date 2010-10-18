require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime#-" do

  it "should substract a number of days from a DateTime" do
    (DateTime.civil(2008, 1, 8) - 315).should == DateTime.civil(2007,2,27)
    (DateTime.commercial(2007, 47, 2) - 315).should == DateTime.commercial(2007,2,2)
    (DateTime.jd(2455097) - 315).should == DateTime.jd(2454782)
    (DateTime.ordinal(2008, 325) - 315).should == DateTime.ordinal(2008, 10)
  end
  
  it "should subtract a fractional number of days to a Date" do
    (DateTime.civil(2008, 1, 8, 12) - 315.5).should == DateTime.civil(2007,2,27)
    (DateTime.commercial(2007, 47, 2, 18) - 315.75).should == DateTime.commercial(2007,2,2)
    (DateTime.jd(2455097, 6) - 315.25).should == DateTime.jd(2454782)
    (DateTime.ordinal(2008, 325, 6) - 315.25).should == DateTime.ordinal(2008, 10)
  end
  
  it "should keep the same class as the receiver" do
    c = Class.new(DateTime)
    c.jd.-(10).should be_kind_of(c)
  end

  it "should substract a negative number of days from a DateTime" do
    d = DateTime.civil(2007, 4, 19).-(-13)
    d.should == DateTime.civil(2007, 5 ,2) 
  end
  
  it "should subtract a fractional negative number of days to a Date" do
    d = DateTime.civil(2007, 2, 16, 12).-(-10.5)
    d.should == DateTime.civil(2007,2,27)
  end

  it "should be able to compute the different between two DateTimes" do
    (DateTime.civil(2007,2,27) - DateTime.civil(2007,2,27)).should be_close(0.0, 0.00000001)
    (DateTime.civil(2007,2,27) - DateTime.civil(2007,2,26)).should be_close(1.0, 0.00000001)
    (DateTime.civil(2006,2,27) - DateTime.civil(2007,2,27)).should be_close(-365.0, 0.00000001)
    (DateTime.civil(2008,2,27) - DateTime.civil(2007,2,27)).should be_close(365.0, 0.00000001)
    (DateTime.civil(2009,2,27) - DateTime.civil(2008,2,27)).should be_close(366.0, 0.00000001)
    
    (DateTime.civil(2009,2,27) - DateTime.commercial(2008,2,1)).should be_close(417.0, 0.00000001)
    (DateTime.civil(2009,2,27) - DateTime.jd(2454782)).should be_close(108.0, 0.00000001)
    (DateTime.civil(2009,2,27) - DateTime.ordinal(2008, 10)).should be_close(414.0, 0.00000001)
    
    (DateTime.commercial(2008,2,1) - DateTime.civil(2008,2,27)).should be_close(-51.0, 0.00000001)
    (DateTime.commercial(2008,2,1) - DateTime.jd(2454782)).should be_close(-309.0, 0.00000001)
    (DateTime.commercial(2008,2,1) - DateTime.ordinal(2008, 10)).should be_close(-3.0, 0.00000001)
    
    (DateTime.jd(2454782) - DateTime.commercial(2008,2,1)).should be_close(309.0, 0.00000001)
    (DateTime.jd(2454782) - DateTime.civil(2009,2,27)).should be_close(-108.0, 0.00000001)
    (DateTime.jd(2454782) - DateTime.ordinal(2008, 10)).should be_close(306.0, 0.00000001)
    
    (DateTime.ordinal(2008, 10) - DateTime.commercial(2008,2,1)).should be_close(3.0, 0.00000001)
    (DateTime.ordinal(2008, 10) - DateTime.jd(2454782)).should be_close(-306.0, 0.00000001)
    (DateTime.ordinal(2008, 10) - DateTime.civil(2009,2,27)).should be_close(-414.0, 0.00000001)
  end
  
  it "should consider the offset when computing the different between two DateTimes" do
    (DateTime.civil(2007,2,27, 0, 0, 0, 0.5) - DateTime.civil(2007,2,27,0,0,0)).should be_close(-0.5, 0.00000001)
    (DateTime.civil(2007,2,27, 0, 0, 0, -0.5) - DateTime.civil(2007,2,27,12,0,0)).should be_close(0.0, 0.00000001)
    (DateTime.civil(2007,2,27,0,0,0,-0.5) - DateTime.civil(2007,2,27,0,0,0,0.5)).should be_close(1.0, 0.00000001)
  end

  it "should be able to subtract a Date from a DateTime" do
    (DateTime.ordinal(2008, 10) - Date.commercial(2008,2,1)).should be_close(3.0, 0.00000001)
    (DateTime.ordinal(2008, 10) - Date.jd(2454782)).should be_close(-306.0, 0.00000001)
    (DateTime.ordinal(2008, 10) - Date.civil(2009,2,27)).should be_close(-414.0, 0.00000001)
  end

  it "should assume a Date is in the same offset as the receiver" do
    (DateTime.ordinal(2008, 10, 0, 0, 0, 0.5) - Date.commercial(2008,2,1)).should be_close(3.0, 0.00000001)
    (DateTime.ordinal(2008, 10, 0, 0, 0, 0.1) - Date.jd(2454782)).should be_close(-306.0, 0.00000001)
    (DateTime.ordinal(2008, 10, 0, 0, 0, -0.5) - Date.civil(2009,2,27)).should be_close(-414.0, 0.00000001)
  end

  it "should raise an error on non numeric parameters" do
    lambda { DateTime.civil(2007,2,27) - :hello }.should raise_error(TypeError)
    lambda { DateTime.civil(2007,2,27) - "hello" }.should raise_error(TypeError)
    lambda { DateTime.civil(2007,2,27) - Object.new }.should raise_error(TypeError)
  end
  
end
