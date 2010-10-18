require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime#succ" do
  it "should be the next day" do
    ds    = DateTime.civil(2008, 10, 11)
    ds.succ.should == DateTime.civil(2008, 10, 12)
    ds    = DateTime.civil(2008, 10, 31)
    ds.succ.should == DateTime.civil(2008, 11, 1)
    ds    = DateTime.commercial(2008, 2, 7)
    ds.succ.should == DateTime.commercial(2008, 3, 1)
    ds    = DateTime.jd(2008)
    ds.succ.should == DateTime.jd(2009)
    ds    = DateTime.ordinal(2008, 366)
    ds.succ.should == DateTime.ordinal(2009, 1)
  end

  it "should keep the same class as the receiver" do
    c = Class.new(DateTime)
    c.jd.succ.should be_kind_of(c)
  end

  it "should be aliased as next" do
    DateTime.civil(2008, 10, 11).next.should == DateTime.civil(2008, 10, 12)
  end

  it "should keep the same fractional component" do
    DateTime.civil(2008, 10, 11, 12, 13, 14).succ.should == DateTime.civil(2008, 10, 12, 12, 13, 14)
  end
end
